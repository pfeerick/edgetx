/*
 * Copyright (C) EdgeTX
 *
 * Based on code named
 *   opentx - https://github.com/opentx/opentx
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "flashfirmwaredialog.h"
#include "ui_flashfirmwaredialog.h"
#include "appdata.h"
#include "helpers.h"
#include "radiointerface.h"
#include "firmwareinterface.h"
#include "progressdialog.h"
#include "progresswidget.h"
#include "splashlibrarydialog.h"
#include "storage.h"
#include "autocollapsiblesection.h"

#include <QTimer>

FlashFirmwareDialog::FlashFirmwareDialog(QWidget * parent) :
  QDialog(parent),
  ui(new Ui::FlashFirmwareDialog),
  fwName(g.profile[g.id()].fwName()),
  imageSource(FIRMWARE),
  imageFile(""),
  connectionMode(CONNECTION_NONE)
{
  ui->setupUi(this);
  ui->checkBackup->setChecked(g.backupOnFlash());

  ui->advancedWidget->setTitle(tr("Advanced"));
  ui->advancedWidget->setAnimationDuration(300);
  QVBoxLayout *advLayout = new QVBoxLayout();
  chkHWComp = new QCheckBox(tr("check hardware compatibility (recommended)"), ui->advancedWidget);
  chkHWComp->setChecked(true);
  advLayout->addWidget(chkHWComp);
  chkProfComp = new QCheckBox(tr("check profile compatibility"), ui->advancedWidget);
  chkProfComp->setChecked(true);
  advLayout->addWidget(chkProfComp);
  ui->advancedWidget->setContentLayout(*advLayout);

  const QString splashFile(g.currentProfile().splashFile());

  if (!splashFile.isEmpty() && QFile(splashFile).exists()){
    imageSource = PROFILE;
    imageFile = splashFile;
    ui->useProfileSplash->setChecked(true);
  } else {
    ui->useProfileSplash->setDisabled(true);
  }

  detectClicked(true);
  updateUI();

  connect(ui->detectButton, &QPushButton::clicked, this, &FlashFirmwareDialog::detectClicked);
  connect(ui->loadButton, &QPushButton::clicked, this, &FlashFirmwareDialog::loadClicked);
  connect(ui->useProfileSplash, &QRadioButton::clicked, this, &FlashFirmwareDialog::useProfileSplashClicked);
  connect(ui->useFirmwareSplash, &QRadioButton::clicked, this, &FlashFirmwareDialog::useFirmwareSplashClicked);
  connect(ui->useLibrarySplash, &QRadioButton::clicked, this, &FlashFirmwareDialog::useLibrarySplashClicked);
  connect(ui->useExternalSplash, &QRadioButton::clicked, this, &FlashFirmwareDialog::useExternalSplashClicked);
  connect(ui->writeButton, &QPushButton::clicked, this, &FlashFirmwareDialog::writeButtonClicked);
  connect(ui->cancelButton, &QPushButton::clicked, [this] () { close(); } );
  connect(ui->advancedWidget, &AutoCollapsibleSection::resized, this, &FlashFirmwareDialog::shrink);
}

FlashFirmwareDialog::~FlashFirmwareDialog()
{
  delete ui;
}

void FlashFirmwareDialog::updateUI()
{
  if (connectionMode)
    ui->loadButton->setEnabled(true);
  else
    ui->loadButton->setEnabled(false);

  if (connectionMode && isFileConnectionCompatible() && QFile(fwName).exists()) {
    ui->firmwareFilename->setText(fwName);
    ui->writeButton->setEnabled(true);

    FirmwareInterface firmware(fwName);

    if (firmware.isValid()) {
      ui->firmwareInfoFrame->show();
      ui->date->setText(firmware.getDate() + " " + firmware.getTime());
      ui->version->setText(firmware.getVersion());
      ui->variant->setText(firmware.getFlavour());

      if (firmware.hasSplash()) {
        ui->splashFrame->show();
        ui->splash->setFixedSize(firmware.getSplashWidth(), firmware.getSplashHeight());

        QImage image;
        switch (imageSource) {
          case FIRMWARE:
            ui->useFirmwareSplash->setChecked(true);
            image = firmware.getSplash();
            break;
          case PROFILE:
            ui->useProfileSplash->setChecked(true);
            image.load(g.currentProfile().splashFile());
            break;
          case LIBRARY:
            ui->useLibrarySplash->setChecked(true);
            image.load(imageFile);
            break;
          case EXTERNAL:
            ui->useExternalSplash->setChecked(true);
            image.load(imageFile);
            break;
        }

        if (!image.isNull())
          ui->splash->setPixmap(makePixMap(image));
        else
          ui->splash->setPixmap(QPixmap());

      } else {
        ui->splashFrame->hide();
      }
    } else {
      ui->firmwareInfoFrame->hide();
      ui->splashFrame->hide();
    }
  } else {
    ui->firmwareFilename->setText("");
    ui->writeButton->setEnabled(false);
    ui->firmwareInfoFrame->hide();
    ui->splashFrame->hide();
  }

  // add resize event to queue after changes above
  QTimer::singleShot(0, [=]() { shrink(); });
}

void FlashFirmwareDialog::loadClicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Firmware File"),
                                                  g.flashDir(),
                                                  getFirmwareFilesFilter());

  if (!fileName.isEmpty()) {
    if (!firmwareFileExtensions().contains(QFileInfo(fileName).suffix().toLower())) {
      QMessageBox::critical(this, tr("Open Firmware File"),
                            tr("%1 has an unsupported file extension")
                            .arg(fileName));
      return;
    }

    FirmwareInterface newfw(fileName);

    if (chkHWComp->isChecked()) {
      if (!newfw.isValid()) {
        QMessageBox::critical(this, tr("Open Firmware File"),
                              tr("Error - %1 is not a valid firmware file")
                              .arg(fileName));
        return;
      }

      if (connectionMode == CONNECTION_UF2) {
        const Uf2Info uf2(getUf2Info());
        if (newfw.getFlavour() != uf2.board) {
          QMessageBox::critical(this, tr("Open Firmware File"),
                                tr("%1 \nIncompatability - File: '%2' Connected radio: '%3'")
                                .arg(fileName)
                                .arg(newfw.getFlavour())
                                .arg(uf2.board));
          return;
        }
      }
    }

    if (chkProfComp->isChecked() &&
        newfw.getFlavour() != getCurrentFirmware()->getFlavour()) {
      QMessageBox::critical(this, tr("Open Firmware File"),
                            tr("%1 \nIncompatability - File: '%2' Profile: '%3'")
                            .arg(fileName)
                            .arg(newfw.getFlavour())
                            .arg(getCurrentFirmware()->getFlavour()));
      return;
    }

    fwName = fileName;
    updateUI();
  }
}

void FlashFirmwareDialog::useFirmwareSplashClicked()
{
  FirmwareInterface firmware(fwName);

  if (!firmware.isValid()) {
    QMessageBox::warning(this, CPN_STR_TTL_ERROR, tr( "The firmware file is not valid." ));
  } else if (!firmware.hasSplash()) {
    QMessageBox::warning(this, CPN_STR_TTL_ERROR,
                         tr( "The firmware file does not cntain a start screen image." ));
  } else {
    imageSource = FIRMWARE;
  }

  updateUI();
}

void FlashFirmwareDialog::useProfileSplashClicked()
{
  QString fileName = g.currentProfile().splashFile();

  if (!fileName.isEmpty()) {
    QImage image(fileName);
    if (image.isNull()) {
      QMessageBox::critical(this, CPN_STR_TTL_ERROR,
                            tr("Profile image %1 is invalid.").arg(fileName));
    } else {
      imageSource = PROFILE;
    }
  }

  updateUI();
}

void FlashFirmwareDialog::useExternalSplashClicked()
{
  QString supportedImageFormats;

  for (int formatIndex = 0; formatIndex < QImageReader::supportedImageFormats().count(); formatIndex++) {
    supportedImageFormats += QLatin1String(" *.") + QImageReader::supportedImageFormats()[formatIndex];
  }

  QString fileName = QFileDialog::getOpenFileName(this, tr("Open image file to use as radio start screen"),
                                                  g.imagesDir(),
                                                  tr("Images (%1)").arg(supportedImageFormats));

  if (!fileName.isEmpty()){
    g.imagesDir( QFileInfo(fileName).dir().absolutePath() );
    QImage image(fileName);

    if (image.isNull()) {
      QMessageBox::critical(this, CPN_STR_TTL_ERROR,
                            tr("Image could not be loaded from %1")
                            .arg(fileName));
    } else {
      imageSource = EXTERNAL;
      imageFile = fileName;
    }
  }

  updateUI();
}

void FlashFirmwareDialog::useLibrarySplashClicked()
{
  QString fileName;
  SplashLibraryDialog(this, &fileName).exec();

  if (!fileName.isEmpty()) {
    QImage image(fileName);

    if (image.isNull()) {
      QMessageBox::critical(this, CPN_STR_TTL_ERROR, tr("The library image could not be loaded"));
    } else {
      imageSource = LIBRARY;
      imageFile = fileName;
    }
  }

  updateUI();
}

void FlashFirmwareDialog::writeButtonClicked()
{
  g.flashDir(QFileInfo(fwName).dir().absolutePath());
  g.currentProfile().fwName(fwName);
  g.backupOnFlash(ui->checkBackup->isChecked());

  FirmwareInterface firmware(fwName);

  // splash customisation only applies to binary firmware that embeds a splash
  // image; UF2 files are always written unmodified
  if (imageSource != FIRMWARE && !firmware.isUF2() && firmware.hasSplash()) {
    // load the splash image
    const QPixmap pixmap = ui->splash->pixmap(Qt::ReturnByValue);
    QImage image;
    image = pixmap.toImage().scaled(ui->splash->width(), ui->splash->height());

    if (image.isNull()) {
      QMessageBox::critical(this, CPN_STR_TTL_WARNING, tr("Splash image not found"));
      return;
    }

    // write the customized firmware
    QString tempFile;
    tempFile = generateProcessUniqueTempFileName("flash.bin");
    qDebug() << "patching" << fwName << "with custom splash screen and saving to" << tempFile;
    firmware.setSplash(image);

    if (firmware.save(tempFile) <= 0) {
      QMessageBox::critical(this, CPN_STR_TTL_WARNING, tr("Cannot save customised firmware"));
      return;
    }

    startWrite(tempFile);
  } else {
    startWrite(fwName);
  }
}

void FlashFirmwareDialog::startWrite(const QString &filename)
{
  bool checkHw = chkHWComp->isChecked();
  bool checkProfile = chkProfComp->isChecked();
  bool backup = g.backupOnFlash();
  FirmwareInterface newfw(filename);

  // UF2 firmware flashed over DFU in two stages (H7 radios with the firmware in
  // external flash). In STM32 ROM DFU the current firmware cannot be reached,
  // so the checks use the version of the bootloader in internal flash instead
  // (see checkAgainstBootloader). When the radio is already in its bootloader's
  // DFU mode, the firmware can be read back directly.
  uint32_t uf2FwAddr = 0;
  const bool uf2OverDfu = newfw.isUF2() && !isUf2DeviceFound() &&
                          getUf2FirmwareAddress(newfw.getRawData(), uf2FwAddr);
  bool inBootloaderDfu = false;
  const bool uf2InStmDfu = uf2OverDfu &&
                           probeDfuAddress(uf2FwAddr, inBootloaderDfu) &&
                           !inBootloaderDfu;

  close();

  ProgressDialog progressDialog(this, tr("Flash Firmware to Radio"),
                                CompanionIcon("write_flash.png"));

  auto progress = progressDialog.progress();

  if (uf2InStmDfu && (checkHw || checkProfile || backup)) {
    checkAgainstBootloader(progress, newfw, checkHw, checkProfile, backup, uf2FwAddr);
  } else if (checkHw || checkProfile || backup) {
    progress->updateInfoAndMessages(tr("Reading old firmware..."));

    FirmwareReadOptions readOptions;
    if (uf2OverDfu) {
      readOptions.uf2Source = newfw.getRawData();
      // the version labels can be anywhere in the image, so the checks need
      // all of it as well, but not the erased rest of the segment
      readOptions.stopAtErased = true;
    }

    readFirmware(
        [this, &newfw, progress, checkHw, checkProfile, backup, uf2FwAddr](
            const QByteArray &_data, bool rebooted) {
          FirmwareInterface currfw(_data);

          qDebug() << "profile:" << getCurrentFirmware()->getFlavour()
                    << "current:" << currfw.getFlavour()
                    << "new:" << newfw.getFlavour();

          if (checkHw) {
            if (!currfw.isValid()) {
              QString errMsg(tr("Firmware read from radio invalid"));
              progress->addMessage(errMsg, QtFatalMsg);
              progress->setInfo(errMsg);
              return;
            }

            progress->addMessage(tr("Performing hardware compatibity check"));
            if (!newfw.isHardwareCompatible(currfw)) {
              QString errMsg(tr("New firmware is not compatible with current firmware"));
              progress->updateInfoAndMessages(errMsg, QtFatalMsg);
              return;
            }
          }

          if (checkProfile) {
            progress->addMessage(tr("Performing profile compatibity check"));
            if (currfw.getFlavour() != getCurrentFirmware()->getFlavour()) {
              QString errMsg(tr("Current firmware is not compatible with profile"));
              progress->updateInfoAndMessages(errMsg, QtFatalMsg);
              return;
            } else if (newfw.getFlavour() != getCurrentFirmware()->getFlavour()) {
              QString errMsg(tr("New firmware is not compatible with profile"));
              progress->updateInfoAndMessages(errMsg, QtFatalMsg);
              return;
            }
          }

          if (backup) {
            progress->addMessage(tr("Backing up current firmware"));
            if (!writeFirmwareToFile(this, _data, progress)) {
              return;
            }
          }

          if (rebooted) {
            if (checkHw || checkProfile) {
              progress->addMessage(tr("Compatibility checks passed"));
            }
            if (!waitForStmDfu(progress, uf2FwAddr)) {
              return;
            }
          }

          progress->addMessage(tr("Flashing new firmware"));
          writeFirmware(newfw.getRawData(), progress);
        },
        [this, &newfw, progress, checkProfile, uf2OverDfu, uf2FwAddr](const QString &err) {
          if (!uf2OverDfu) {
            progress->addMessage(tr("Could not read current firmware: %1").arg(err));
            return;
          }

          progress->addMessage(tr("Could not read current firmware: %1").arg(err),
                               QtWarningMsg);
          flashWithoutReadBack(progress, newfw, checkProfile, uf2FwAddr);
        },
        progress, readOptions);
  } else {
    progress->setInfo(tr("Flashing new firmware..."));
    writeFirmware(newfw.getRawData(), progress);
  }

  progressDialog.exec();

  if (isTempFileName(filename)) {
    qunlink(filename);
  }
}

namespace {

// the radio itself only looks for its version in the first 1 KiB of the image
// (getFirmwareVersion() in radio/src/stamp.cpp)
constexpr uint32_t BOOTLOADER_VERSION_READ_LENGTH {1024};

// boot_version from radio/src/stamp.cpp: "edgetx-<flavour>-<version> (<git>)"
QString findEdgeTxVersion(const QByteArray &data)
{
  const int start = data.indexOf("edgetx-");
  if (start < 0)
    return QString();

  const int end = data.indexOf('\0', start);
  if (end < 0 || end - start > 100)
    return QString();

  return QString::fromLatin1(data.mid(start, end - start));
}

// flavour names never contain '-'
QString flavourFromVersion(const QString &version)
{
  const QStringList parts = version.split('-');
  return parts.size() >= 3 ? parts[1] : QString();
}

}  // namespace

// Checks for a two-stage UF2 with the radio in STM32 ROM DFU. That DFU cannot
// reach the current firmware in external flash, but it can read the EdgeTX
// bootloader in internal flash, whose version names the radio's flavour just
// as the firmware does. Reading it needs no reboot, so the bootloader and the
// firmware can then both be flashed straight away.
void FlashFirmwareDialog::checkAgainstBootloader(ProgressWidget *progress,
                                                 const FirmwareInterface &newfw,
                                                 bool checkHw, bool checkProfile,
                                                 bool backup, uint32_t uf2FwAddr)
{
  if (!checkHw && !checkProfile) {
    backupAndFlash(progress, newfw, uf2FwAddr);
    return;
  }

  progress->updateInfoAndMessages(tr("Reading bootloader version..."));

  FirmwareReadOptions readOptions;
  readOptions.maxLength = BOOTLOADER_VERSION_READ_LENGTH;
  const FirmwareInterface *fw = &newfw;

  readFirmware(
      [this, fw, progress, checkHw, checkProfile, backup, uf2FwAddr](
          const QByteArray &data, bool) {
        const QString version = findEdgeTxVersion(data);
        const QString flavour = flavourFromVersion(version);

        qDebug() << "profile:" << getCurrentFirmware()->getFlavour()
                 << "bootloader:" << version
                 << "new:" << fw->getFlavour();

        if (flavour.isEmpty()) {
          progress->addMessage(tr("No EdgeTX bootloader version found on the radio"),
                               QtWarningMsg);
          flashWithoutReadBack(progress, *fw, checkProfile, uf2FwAddr);
          return;
        }

        progress->addMessage(tr("Radio bootloader: %1").arg(version));

        if (checkHw) {
          progress->addMessage(tr("Performing hardware compatibity check"));
          if (!fw->getFlavour().isEmpty() && fw->getFlavour() != flavour) {
            QString errMsg(tr("New firmware is not compatible with the radio (%1)")
                               .arg(flavour));
            progress->updateInfoAndMessages(errMsg, QtFatalMsg);
            return;
          }
        }

        if (checkProfile) {
          progress->addMessage(tr("Performing profile compatibity check"));
          if (flavour != getCurrentFirmware()->getFlavour()) {
            QString errMsg(tr("Radio is not compatible with profile"));
            progress->updateInfoAndMessages(errMsg, QtFatalMsg);
            return;
          } else if (fw->getFlavour() != getCurrentFirmware()->getFlavour()) {
            QString errMsg(tr("New firmware is not compatible with profile"));
            progress->updateInfoAndMessages(errMsg, QtFatalMsg);
            return;
          }
        }

        if (backup) {
          backupAndFlash(progress, *fw, uf2FwAddr);
        } else {
          progress->addMessage(tr("Flashing new firmware"));
          writeFirmware(fw->getRawData(), progress);
        }
      },
      [this, fw, progress, checkProfile, uf2FwAddr](const QString &err) {
        progress->addMessage(tr("Could not read the radio's bootloader: %1").arg(err),
                             QtWarningMsg);
        flashWithoutReadBack(progress, *fw, checkProfile, uf2FwAddr);
      },
      progress, readOptions);
}

// Backup for a two-stage UF2 with the radio in STM32 ROM DFU. The firmware can
// only be read once the radio's bootloader is running in DFU mode, and that
// cannot write the bootloader, so the radio then has to be put back into DFU
// mode before flashing. Ask each time whether that is worth it.
void FlashFirmwareDialog::backupAndFlash(ProgressWidget *progress,
                                         const FirmwareInterface &newfw,
                                         uint32_t uf2FwAddr)
{
  QMessageBox box(QMessageBox::Question, tr("Backup Current Firmware"),
                  tr("To back up the current firmware, the radio's bootloader "
                     "has to be started in DFU mode, which cannot update the "
                     "bootloader.\n\n"
                     "After the backup, you will be asked to turn the radio off "
                     "and put it back in DFU mode to flash the new firmware.\n\n"
                     "Skip Backup flashes the new firmware now, without a backup."),
                  QMessageBox::Cancel, progress);
  QPushButton *backupFirst = box.addButton(tr("Back Up First"), QMessageBox::AcceptRole);
  QPushButton *skipBackup = box.addButton(tr("Skip Backup"), QMessageBox::ActionRole);
  box.setDefaultButton(backupFirst);
  box.exec();

  if (box.clickedButton() == skipBackup) {
    progress->addMessage(tr("Backup of current firmware skipped"), QtWarningMsg);
    progress->addMessage(tr("Flashing new firmware"));
    writeFirmware(newfw.getRawData(), progress);
    return;
  } else if (box.clickedButton() != backupFirst) {
    progress->updateInfoAndMessages(tr("Flashing cancelled"), QtWarningMsg);
    return;
  }

  progress->updateInfoAndMessages(tr("Reading old firmware..."));

  FirmwareReadOptions readOptions;
  readOptions.uf2Source = newfw.getRawData();
  readOptions.stopAtErased = true;
  const FirmwareInterface *fw = &newfw;

  readFirmware(
      [this, fw, progress, uf2FwAddr](const QByteArray &data, bool) {
        progress->addMessage(tr("Backing up current firmware"));
        if (!writeFirmwareToFile(this, data, progress)) {
          return;
        }

        if (!waitForStmDfu(progress, uf2FwAddr)) {
          return;
        }

        progress->addMessage(tr("Flashing new firmware"));
        writeFirmware(fw->getRawData(), progress);
      },
      [this, fw, progress, uf2FwAddr](const QString &err) {
        progress->addMessage(tr("Could not read current firmware: %1").arg(err),
                             QtWarningMsg);

        const auto answer = QMessageBox::warning(
            progress, CPN_STR_TTL_WARNING,
            tr("The current firmware could not be read, so it cannot be backed "
               "up.\n\nContinue flashing without a backup?"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (answer != QMessageBox::Yes) {
          progress->updateInfoAndMessages(tr("Flashing cancelled"), QtWarningMsg);
          return;
        }

        progress->addMessage(tr("Backup of current firmware skipped"), QtWarningMsg);

        // the radio may have rebooted out of DFU mode while trying to start
        // its bootloader in DFU mode
        if (!waitForStmDfu(progress, uf2FwAddr)) {
          return;
        }

        progress->addMessage(tr("Flashing new firmware"));
        writeFirmware(fw->getRawData(), progress);
      },
      progress, readOptions);
}

// The read-back failed for a two-stage UF2 flashed over DFU, e.g. because the
// radio's bootloader could not be read, or is too old to start in DFU mode on
// request. Offer to flash without the hardware check and backup, as the new
// firmware can still be checked against the profile.
void FlashFirmwareDialog::flashWithoutReadBack(ProgressWidget *progress,
                                               const FirmwareInterface &newfw,
                                               bool checkProfile,
                                               uint32_t uf2FwAddr)
{
  const auto answer = QMessageBox::warning(
      progress, CPN_STR_TTL_WARNING,
      tr("The firmware currently on the radio could not be read, so the "
         "hardware compatibility check and the backup cannot be performed.\n\n"
         "Make sure the selected firmware is for this radio before "
         "continuing.\n\n"
         "Continue flashing?"),
      QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

  if (answer != QMessageBox::Yes) {
    progress->updateInfoAndMessages(tr("Flashing cancelled"), QtWarningMsg);
    return;
  }

  progress->addMessage(tr("Hardware compatibility check and backup skipped"),
                       QtWarningMsg);

  if (checkProfile) {
    // only the new firmware can be checked against the profile
    progress->addMessage(tr("Performing profile compatibity check"));
    if (newfw.getFlavour() != getCurrentFirmware()->getFlavour()) {
      QString errMsg(tr("New firmware is not compatible with profile"));
      progress->updateInfoAndMessages(errMsg, QtFatalMsg);
      return;
    }
  }

  // the radio may have rebooted out of DFU mode while trying to start its
  // bootloader in DFU mode
  if (!waitForStmDfu(progress, uf2FwAddr)) {
    return;
  }

  progress->addMessage(tr("Flashing new firmware"));
  writeFirmware(newfw.getRawData(), progress);
}

// Make sure the radio is in STM32 (ROM) DFU mode, where both the bootloader
// and the firmware of a two-stage UF2 can be written: the device is present
// and does not expose the firmware address. Returns false if the user cancels.
bool FlashFirmwareDialog::waitForStmDfu(ProgressWidget *progress, uint32_t uf2FwAddr)
{
  while (true) {
    bool inBootloaderDfu = false;
    const bool found = probeDfuAddress(uf2FwAddr, inBootloaderDfu);

    if (found && !inBootloaderDfu) {
      return true;
    }

    QMessageBox box(QMessageBox::Information, tr("Flash Firmware to Radio"),
                    QString(), QMessageBox::Retry | QMessageBox::Cancel, progress);
    box.setDefaultButton(QMessageBox::Retry);
    QPushButton *firmwareOnly = nullptr;

    if (found) {
      box.setText(tr("The radio is in its bootloader's DFU mode, which can "
                     "update the firmware but not the bootloader.\n\n"
                     "To update both, turn the radio off, put it back in DFU "
                     "mode the same way as before, then click Retry."));
      firmwareOnly = box.addButton(tr("Flash Firmware Only"), QMessageBox::AcceptRole);
    } else {
      box.setText(tr("No radio found in DFU mode.\n\n"
                     "Turn the radio off, put it back in DFU mode the same way "
                     "as before, then click Retry."));
    }

    box.exec();

    if (firmwareOnly && box.clickedButton() == firmwareOnly) {
      return true;
    } else if (box.clickedButton() != box.button(QMessageBox::Retry)) {
      progress->updateInfoAndMessages(tr("Flashing cancelled"), QtWarningMsg);
      return false;
    }
  }
}

void FlashFirmwareDialog::detectClicked(bool atLoad)
{
  QString connectionMsg;

  if (isUf2DeviceFound()) {
    connectionMode = CONNECTION_UF2;
    connectionMsg = tr("Radio connection mode: UF2");
  } else if (isDfuDeviceFound()) {
    connectionMode = CONNECTION_DFU;
    connectionMsg = tr("Radio connection mode: DFU");
  } else {
    connectionMode = CONNECTION_NONE;
    connectionMsg = tr("ALERT: No radio detected");
  }

  ui->connectionMode->setText(connectionMsg);

  if (!connectionMode) {
    QMessageBox::warning((atLoad ? (QWidget *)parent(): this), tr("Detect Radio"),
      tr("Radio could not be detected by DFU or UF2 modes") % ".\n" %
      tr("Check cable is securely connected and radio lights are illuminated") % ".\n" %
      tr("Note: USB mode is not suitable for flashing firmware."));
  }

  if (!atLoad)
    updateUI();
}

bool FlashFirmwareDialog::isFileConnectionCompatible()
{
  if (!fwName.isEmpty()) {
    const QString extn(QFileInfo(fwName).suffix().toLower());
    if ((connectionMode == CONNECTION_DFU && dfuFileExtensions().contains(extn)) ||
        (connectionMode == CONNECTION_UF2 && uf2FileExtensions().contains(extn))) {
      return true;
    }
  }

  return false;
}

void FlashFirmwareDialog::shrink()
{
  this->adjustSize();
  this->resize(0, 0);
}
