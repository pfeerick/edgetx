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

#pragma once

#include "rs_dfu.h"

#include <QThread>
#include <QString>

#include <set>

using SliceU8 = rust::Slice<const uint8_t>;
using ::rust::Vec;

// How to read the current firmware back from the radio
struct FirmwareReadOptions {
  // UF2 file about to be flashed. Over DFU, when set and the device does not
  // expose the UF2 firmware address (STM32 ROM DFU on H7 radios with the
  // firmware in external flash), the UF2 reboot block is sent on its own to
  // start the radio's EdgeTX bootloader in DFU mode, and the firmware is read
  // from there. Nothing is written to flash.
  QByteArray uf2Source;
  // bytes to read from the device's default start address, 0 reads the whole
  // segment (not used when reading the UF2 firmware address)
  uint32_t maxLength = 0;
  // stop reading at the first 64 KiB of erased flash and drop the trailing
  // erased bytes, instead of reading the whole segment (only applies when
  // reading the UF2 firmware address)
  bool stopAtErased = false;
};

class FirmwareReaderWorker : public QThread
{
  Q_OBJECT

  public:
    explicit FirmwareReaderWorker(QObject *parent = nullptr,
                                  const FirmwareReadOptions &options = {});
    ~FirmwareReaderWorker();

  protected:
    void run() override;

  signals:
    void newMessage(const QString &msg);
    void updateMessage(const QString &msg);
    void progressChanged(int value, int total);
    void statusChanged(const QString &status);
    void error(const QString &error);
    // the radio was rebooted from ROM DFU into its bootloader's DFU to read
    // the firmware, emitted before complete()
    void rebootedForRead();
    void complete(const QByteArray& data);

  private:
    FirmwareReadOptions options;

    void runDfu();
    void runUf2();
};

class FirmwareWriterWorker : public QThread
{
  Q_OBJECT

  private:
    QByteArray firmwareData;

  public:
    explicit FirmwareWriterWorker(const QByteArray &data, QObject *parent = nullptr);
    ~FirmwareWriterWorker();

  protected:
    void run() override;

  signals:
    void newMessage(const QString &msg);
    void updateMessage(const QString &msg);
    void progressChanged(int value, int total);
    void statusChanged(const QString &status);
    void error(const QString &error);
    void warning(const QString &msg);
    void complete();

  private:
    void writeUf2(DfuDevice &device, const SliceU8 &data);
    void writeRegion(const DfuDevice &device, uint32_t addr, const SliceU8 &data,
                     std::set<uint32_t> &erased_pages);
    void updateEraseStatus(size_t page, size_t pages);
    void updateDownloadStatus(size_t bytes, size_t total);
    void runDfu();
    void runUf2();

    template <typename Duration>
    void rebootAndRediscover(DfuDevice &device, uint32_t addr,
                                const SliceU8 &data, uint32_t reboot_addr,
                                Duration timeout);
};


class ProgressWidget;

//  TODO make a class
struct Uf2Info {
  QString version;
  QString board;
  QString date;
};

QString findMassStoragePath(const QString &filename, bool onlyPath = false, ProgressWidget *progress = nullptr);

bool readFirmware(const std::function<void(const QByteArray &)>& onComplete,
                  const std::function<void(const QString &)>& onError,
                  ProgressWidget *progress);

// onComplete's second argument is true when the radio was rebooted from ROM
// DFU into its bootloader's DFU to read the firmware (see FirmwareReadOptions)
bool readFirmware(const std::function<void(const QByteArray &, bool)>& onComplete,
                  const std::function<void(const QString &)>& onError,
                  ProgressWidget *progress, const FirmwareReadOptions &options);

bool readFirmware(QByteArray &data, ProgressWidget *progress);
bool readFirmware(const QString &filename, ProgressWidget *progress);

bool writeFirmware(const QByteArray &data, ProgressWidget *progress);
bool writeFirmware(const QString &filename, ProgressWidget *progress);

bool readSettings(const QString &filename, ProgressWidget *progress);
bool readSettingsSDCard(const QString &filename, ProgressWidget *progress, bool fromRadio = true);

bool writeSettings(const QString &filename, ProgressWidget *progress);

QString printDevicesInfo(const Vec<DfuDevice> &devices);
QString getDevicesInfo();
QString getFirmwareFilesFilter();
Uf2Info getUf2Info();
bool isDfuDeviceFound();
// Address the UF2 firmware is written to after the reboot block, i.e. the
// address that is only reachable once the radio's bootloader is running.
// Returns false when the UF2 has no reboot block followed by firmware.
bool getUf2FirmwareAddress(const QByteArray &uf2, uint32_t &addr);
// Returns false unless exactly one DFU device is present; otherwise sets
// covered to whether that device exposes a memory segment containing addr.
bool probeDfuAddress(uint32_t addr, bool &covered);
bool isUf2DeviceFound();
bool isRadioConnected();
bool writeFirmwareToFile(QWidget *parent, const QByteArray &data, ProgressWidget *progress, bool promptForFile = false);
QStringList dfuFileExtensions();
QStringList uf2FileExtensions();
QStringList firmwareFileExtensions();
