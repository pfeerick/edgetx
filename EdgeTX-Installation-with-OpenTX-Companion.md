<p align="center">
<a href="url"><img src="https://github.com/EdgeTX/edgetx.github.io/blob/master/images/edgetx-v2.png" align="center" height="150" width="150" ></a>

# EdgeTX Installation with OpenTX Companion


_Note: Before you update to EdgeTX, it is highly recommended that you **backup your current firmware and SD card contents in case you need to roll back** to your previous version for some reason. Additionally, **you will need your MODEL, RADIO folders if you want to convert your existing models** created in OpenTX to work with EdgeTX. Also, if you have added custom sounds, model images widgets or Lua scripts to your radio, you will also need your SOUNDS, WIDGETS, and SCRIPTS folders._

_Unfortunately, it is not possible to copy over the setup for your widgets from OpenTX. They will have to be set up again manually in EdgeTX._

## SD Card Preparation
1. Download and extract the EdgeTX SD card content for your radio type to your computer. The SD card contents can be found here: https://github.com/EdgeTX/edgetx-sdcard/releases

The list below shows which .zip file to use for different radio types:

* horus.zip (480x272 Horizontal Color Screen) - TX16s, T16, Horus x10s,Horus x12s, Most color screen radios...

* nv14.zip - FlySky Nirvana NV14 Note: this transmitter is not yet supported in EdgeTX 2.4.0 and is planned for future release 

* taranis-x7.zip (128x64 BW Screens) -T-Lite, QX7, X9 Lite, X-Lite, X-Lite Pro

* taranis-x9.zip (212x64 BW Screens) - X9D, X9D Plus, X9D Plus 2019

2. Delete everything from your SD card and copy the contents of the unzipped folder to your blank SD card.  (If you did a format ensure it is set to fat32)

Note: To import your models from OpenTX, copy your MODEL, RADIO and (optional) SOUND, WIDGET, and SCRIPT folders folders along with their COMPLETE contents from your OpenTX SD Card to your new EdgeTX SD Card.




8. Unplug your USB cable from your Radio and power your radio off.
## Flashing the EdgeTX Bootloader and Firmware

1. Open EdgeTX flasher if it is not still open.
2. Plug in your radio (radio power should be off).
3. Select “Flash” from the menu on the left side of the screen.
4. Select “edgetx-firmware-v2.4.0rc3” from the option “Firmware Branch”
5. Select the latest firmware for the “Firmware Version”
6. Once metadata is downloaded, select the **CORRECT** radio type that you intend to flash.
7. Select “Write”. You will see a message “Downloading bin” and then a “Flashing” popup with the flashing progress. Eventually, you will see “File downloaded successfully”. Ignore any error messages after that and click on "Close".
8. After the flashing is complete, restart your radio and you should be greeted with "Welcome to EdgeTX."

**If you did not add the RADIO and MODELS folders to your SD card for conversion**, when the radio starts with EdgeTX for the first time, you will get a STORAGE WARNING - Bad Radio Data-. Press the white circle or roller to bypass the warning. Then you will get another STORAGE WARNING - Storage Preparation. Press the white circle or roller again. Once the SD card is prepared, the calibration screen will appear. Calibrate your radio.

**If you did add the RADIO and MODELS folders to your SD card for conversion** you should see a message telling you that a SD conversion is necessary. Select the white circle and it will begin converting your models. Depending on your previous radio settings the calibration screen may appear, If so, Calibrate your radio.

# You are ready to start using EdgeTX!

_**Note 1: The radio software may default to mode 1 stick configuration. If you have a mode 2 radio, you will most likely see a “Throttle Warning”. To fix this, go into the radio settings and change to mode 2.**_

## Installation Reference Videos

[EdgeTX • How-To Migrate from OpenTX to EdgeTX and back to OpenTX](https://www.youtube.com/watch?v=PhpDf-j_3_Q) - RC Video Reviews