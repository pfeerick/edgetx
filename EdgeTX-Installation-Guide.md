<p align="center">
<a href="url"><img src="https://github.com/EdgeTX/edgetx.github.io/blob/master/images/edgetx-v2.png" align="center" height="150" width="150" ></a>

# EdgeTX Installation Guide
As of 28.June.2021


_Note: Before you update to EdgeTX, it is highly recommended that you **backup your current firmware and SD card contents in case you need to roll back** to your previous version for some reason. Additionally, **you will need your MODEL, RADIO folders if you want to convert your existing models** created in OpenTX to work with EdgeTX. Also, if you have added custom sounds, model images widgets or Lua scripts to your radio, you will also need your SOUNDS, WIDGETS, and SCRIPTS folders_

## Installing the EdgeTX Flasher
1. Download the EdgeTX firmware flasher utility from the following link:  https://github.com/EdgeTX/flasher/releases
2. Click the arrow to the left side of “Assets” to expand the menu.
3. Click on the correct executable file based on your operating system (Linux, Mac, Windows) to download the file.
4. Depending on your web browser, you may get a warning that the file type may be dangerous. If this occurs, select the option to save/keep the file anyways (This warning is normal when downloading executable files).
5. Open the executable file that you downloaded. If using Microsoft Windows, you will get a warning, select “More Info” and “Run Anyway”. It will install the EdgeTX Flasher program and then open it (it may take a little while to install ~45s).

## SD Card Preparation
1. Power on your radio, connect to your PC with a data-capable USB cable, and select “USB mode” when prompted by the radio.
2. Select “SD Card” from the menu on the left side.
3. Select the correct radio target from the drop-down menu that corresponds to your radio type.
4. Select the desired voice pack(s) that you want to install.
5. Select the correct disk that corresponds to the SD card on your radio. The flasher program will only detect USB-connected drives.  
 
_Note: It is recommended that no other USB drives are connected to your PC at this time in order to prevent accidental data loss from selecting the incorrect USB drive._

6. Check the checkbox “Erase disk before flashing” to erase the contents of your SD if your SD card is not already empty.
7. Select “Write to SD card” and the SD card data will be downloaded and copied to your selected USB device. 

_Note: To import your models from OpenTX, copy your MODELS and RADIO folders along with their **COMPLETE** contents from your OpenTX SD Card to your new EdgeTX SD Card. Now is also the time to copy over any custom sounds, model images, widgets or Lua scripts to their respective folders._

8. Unplug your USB cable from your Radio and power your radio off.
## Flashing the EdgeTX Bootloader and Firmware

1. Open EdgeTX flasher if it is not still open.
2. Plug in your radio (radio power should be off).
3. Select “Flash” from the menu on the left side of the screen.
4. Select “edgetx-firmware-v2.4.0rc3” from the option “Firmware Branch”
5. Select the latest firmware for the “Firmware Version”
6. Once metadata is downloaded, select the CORRECT radio type that you intend to flash.
7. Select “Write”. You will see a message “Downloading bin” and then a “Flashing” popup with the flashing progress. Eventually, you will see “File downloaded successfully”. Ignore any error messages after that and click on "Close".
8. After the flashing is complete, restart your radio and you should be greeted with "Welcome to EdgeTX."

**If you did not add the RADIO and MODELS folders to your SD card for conversion**, when the radio starts with EdgeTX for the first time, you will get a STORAGE WARNING - Bad Radio Data-. Press the white circle or roller to bypass the warning. Then you will get another STORAGE WARNING - Storage Preparation. Press the white circle or roller again. Once the SD card is prepared, the calibration screen will appear. Calibrate your radio.

**If you did add the RADIO and MODELS folders to your SD card for conversion** you should see a message telling you that a SD conversion is necessary. Select the white circle and it will begin converting your models. Depending on your previous radio settings the calibration screen may appear, If so, Calibrate your radio.

# You are ready to start using EdgeTX!

_**Note 1: The radio software may default to mode 1 stick configuration. If you have a mode 2 radio, you will most likely see a “Throttle Warning”. To fix this, go into the radio settings and change to mode 2.**_

