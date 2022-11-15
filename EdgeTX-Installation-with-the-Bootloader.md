<p align="center">
<a href="url"><img src="https://github.com/EdgeTX/edgetx.github.io/blob/master/images/edgetx-v2.png" align="center" height="150" width="150" ></a>

# EdgeTX Installation with the Bootloader

_Note: Before you update to EdgeTX, it is highly recommended that you **backup your current firmware and SD card contents in case you need to roll back** to your previous version for some reason. Additionally, **you will need your MODEL, RADIO folders if you want to convert your existing models** created in OpenTX to work with EdgeTX. Also, if you have added custom sounds, model images widgets or Lua scripts to your radio, you will also need your SOUNDS, WIDGETS, and SCRIPTS folders._

_Unfortunately, it is not possible to copy over the setup for your widgets from OpenTX. They will have to be set up again manually in EdgeTX._


## Flashing the EdgeTX Bootloader and Firmware

1. Download the current EdgeTX firmware. You can download the latest release .zip file (edgetx-firmware-vX.X.X.zip) directly from Github - https://github.com/EdgeTX/edgetx/releases/latest

2. Unzip the file and save the correct .bin file (same name as your radio type) to the "Firmware" folder on the SD card for your radio.

3. Turn on your radio and navigate to the SD card screen. Open the "Firmware" folder and select the EdgeTX firmware file that you just copied to your SD card. Once the file is selected, select the option to "Flash bootloader". The bootloader will be flashed to the EdgeTX bootloader.

4. Exit back to the main screen and then shut off your radio.

5. Boot your radio in bootloader mode (on most radios, hold trim switches T4 and T1 to center while turning the power on). 

6. You should now see the EdgeTX bootloader. Select the option "Write Firmware".  Select the EdgeTX firmware file that you saved to your SD card. Long-press to flash it.

7. After the flashing is complete, power down the radio. Do not start the radio up until you have completed the SD Card Preparation steps below.


## SD Card Preparation
1. Download and extract the EdgeTX SD card content for your radio type to your computer. The SD card contents can be found here: https://github.com/EdgeTX/edgetx-sdcard/releases

> The list below shows which .zip file to use for different radio types:

> * c480x272.zip (480x272 Horizontal Color Screen) - e.g. TX16S, T16, X10S, X12S (most color screen radios)

> * c320x480.zip (320x480 Portrait Color Screen) - e.g. Flysky Nirvana NV14, Flysky Elysium EL18

> * c480x320.zip (480x320 Horizontal Color Screen) - e.g. Flysky Paladin PL18/PL18EV

> * bw128x64.zip  (128x64 BW Screens) - e.g. T-Lite, Zorro, TX12, LiteRadio3 Pro, QX7, X9 Lite, X-Lite (most BW screen radios)

> * bw212x64.zip  (212x64 BW Screens) - e.g. X9D, X9D Plus, X9D Plus 2019

2. Delete everything from your SD card and copy the contents of the unzipped folder to your blank SD card.  (If you did a format ensure it is set to fat32)

_Note: To import your models from OpenTX, copy your MODELS and RADIO folders along with their COMPLETE contents from your OpenTX SD Card to your new EdgeTX SD Card. Now is also the time to copy over any custom sounds, model images, widgets or Lua scripts to their respective folders._

3. Download the desired sound pack (if you didn’t transfer your existing sounds) (https://github.com/EdgeTX/edgetx-sdcard-sounds/releases), unzip and copy to the `SOUNDS` folder on your SD card. Double check that the folder inside the `SOUNDS` has only two letters (i.e. it should be `en`, not `en_libby`). 

4. Power on your radio. 

**If you did add the RADIO and MODELS folders to your SD card for conversion** you should see a message telling you that a SD conversion is necessary. Touch the screen or press Enter, and it will begin converting your models. Depending on your previous radio settings the calibration screen may appear, If so, Calibrate your radio.

**If you did not add the RADIO and MODELS folders to your SD card for conversion**, when the radio starts with EdgeTX for the first time, you will get a STORAGE WARNING - Bad Radio Data-. Press the white circle or roller to bypass the warning. Then you will get another STORAGE WARNING - Storage Preparation. Press the white circle or roller again. Once the SD card is prepared, the calibration screen will appear. Calibrate your radio.

_**Note 1: The radio software may default to mode 1 stick configuration. If you have a mode 2 radio, you will most likely see a “Throttle Warning”. To fix this, go into the radio settings and change to mode 2.**_


## Installation Reference Videos

[EdgeTX 2.5 means it's time to switch away from OpenTX. Here's how.](https://www.youtube.com/watch?v=shmse1VBiaA) - Joshua Bardwell

[EdgeTX Bootloader Masterclass • Learn How to Flash Your Radio Like the Pros](https://youtu.be/LItyAkJlcdU) - RC Video Reviews

[EdgeTX • How-To Migrate from OpenTX to EdgeTX and back to OpenTX](https://www.youtube.com/watch?v=PhpDf-j_3_Q) - RC Video Reviews

[EdgeTX new cutting edge firmware for OpenTX radios](https://www.youtube.com/watch?v=7prdIFzm2D0) - Joshua Bardwell
