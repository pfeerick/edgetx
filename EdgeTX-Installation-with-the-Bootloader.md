<p align="center">
<a href="url"><img src="https://github.com/EdgeTX/edgetx.github.io/blob/master/images/edgetx-v2.png" align="center" height="150" width="150" ></a>

# EdgeTX Installation with the Bootloader

_Note: Before you update to EdgeTX, it is highly recommended that you **backup your current firmware and SD card contents in case you need to roll back** to your previous version for some reason. Additionally, **you will need your MODEL, RADIO folders if you want to convert your existing models** created in OpenTX to work with EdgeTX. Also, if you have added custom sounds, model images widgets or Lua scripts to your radio, you will also need your SOUNDS, WIDGETS, and SCRIPTS folders._

_Unfortunately, it is not possible to copy over the setup for your widgets from OpenTX. They will have to be set up again manually in EdgeTX._

## SD Card Preparation
1. Download and extract the EdgeTX SD card content for your radio type to your computer. The SD card contents can be found here: https://github.com/EdgeTX/edgetx-sdcard/releases

> The list below shows which .zip file to use for different radio types:

> * horus.zip (480x272 Horizontal Color Screen) - TX16s, T16, Horus x10s,Horus x12s, Most color screen radios...

> * nv14.zip - FlySky Nirvana NV14 Note: this transmitter is not yet supported in EdgeTX 2.4.0 and is planned for future release 

> * taranis-x7.zip (128x64 BW Screens) -T-Lite, QX7, X9 Lite, X-Lite, X-Lite Pro

> * taranis-x9.zip (212x64 BW Screens) - X9D, X9D Plus, X9D Plus 2019

2. Delete everything from your SD card and copy the contents of the unzipped folder to your blank SD card.  (If you did a format ensure it is set to fat32)

_Note: To import your models from OpenTX, copy your MODELS and RADIO folders along with their COMPLETE contents from your OpenTX SD Card to your new EdgeTX SD Card. Now is also the time to copy over any custom sounds, model images, widgets or Lua scripts to their respective folders._

3. Download the desired sound pack (if you didn’t transfer your existing sounds) (https://github.com/EdgeTX/edgetx-sdcard-sounds/releases), unzip and copy to the "Sounds" folder on your SD card.


## Flashing the EdgeTX Bootloader and Firmware

1. Download the current EdgeTX firmware. You can download the latest release .zip file (edgetx-firmware-v2.4.0.zip) directly from Github -https://github.com/EdgeTX/edgetx/releases/tag/v2.4.0

2. Unzip the file and save the correct .bin file (same name as your radio type) to the "Firmware" folder on the SD card for your radio.

3. Turn on your radio and navigate to the SD card screen. Open the "Firmware" folder and select the EdgeTX firmware file that you just copied to your SD card. Once the file is selected, select the option to "Flash bootloader". The bootloader will be flashed to the EdgeTX bootloader.

4. Exit back to the main screen and then shut off your radio.

5. Boot your radio in bootloader mode (holding trim switches T4 and T1 to center while pushing the power button on). 

6. You should now see the EdgeTX bootloader. Select the option "Write Firmware".  Select the EdgeTX firmware file that you saved to your SD card. Long-press to flash it.

7. After the flashing is complete, select "Exit". The radio will restart and you should be greeted with "Welcome EdgeTX". 

**If you did not add the RADIO and MODELS folders to your SD card for conversion**, when the radio starts with EdgeTX for the first time, you will get a STORAGE WARNING - Bad Radio Data-. Press the white circle or roller to bypass the warning. Then you will get another STORAGE WARNING - Storage Preparation. Press the white circle or roller again. Once the SD card is prepared, the calibration screen will appear. Calibrate your radio.

**If you did add the RADIO and MODELS folders to your SD card for conversion** you should see a message telling you that a SD conversion is necessary. Select the white circle and it will begin converting your models. Depending on your previous radio settings the calibration screen may appear, If so, Calibrate your radio.

## You are ready to start using EdgeTX!

_**Note 1: The radio software may default to mode 1 stick configuration. If you have a mode 2 radio, you will most likely see a “Throttle Warning”. To fix this, go into the radio settings and change to mode 2.**_



