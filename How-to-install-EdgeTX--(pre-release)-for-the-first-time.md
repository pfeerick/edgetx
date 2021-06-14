<p align="center">
<a href="url"><img src="https://github.com/EdgeTX/edgetx.github.io/blob/master/images/edgetx-v2.png" align="center" height="150" width="150" ></a>

# How to install EdgeTX for the first time (pre-release)
As of 14.June.2021

### Warning: the EdgeTX firmware is still in a pre-release state and should only be used for testing purposes. It may contain issues - Use at your own risk!
_**Note 1: Before you update to EdgeTX, it is highly recommended that you backup your current firmware and SD card contents in case you need to roll back to your previous version for some reason. Additionally, you will need your MODEL and RADIO folders if you want to convert your existing models created in OpenTX to work with EdgeTX.**_


1. Download and extract the EdgeTX SD card content for your radio type to your computer. The SD card contents can be found here: https://github.com/EdgeTX/edgetx-sdcard/releases

> The list below shows which .zip file to use for different radio types:
> * **horus.zip** (480x272 Horizontal Color Screen) - TX16s, T16, Horus x10s,Horus x12s, Most color screen radios...
> * **nv14.zip** - FlySky Nirvana NV14
> * **taranis-x7.zip** (128x64 BW Screens) -T-Lite, QX7, X9 Lite, X-Lite, X-Lite Pro
> * **taranis-x9.zip** (212x64 BW Screens) - X9D, X9D Plus, X9D Plus 2019

2. Delete everything from your SD card and copy the contents of the unzipped folder to your blank SD card.

> _**Note:**_ To import your models from OpenTX, copy your MODELS and RADIO folders from your OpenTX SD Card to your new EdgeTX SD Card.

3. Download the EdgeTX firmware flash utility: https://github.com/EdgeTX/flasher/releases

4. Click the arrow to the left side of “Assets” to expand the menu.

5. Click on the correct executable file based on your operating system (Linux, Mac, Windows) to download the file.

6. Depending on your web browser, you may get a warning that the file type may be dangerous. If this occurs, select the option to save / keep the file 
   anyways (This warning is normal when downloading executable files). 

7. Open the executable file that you downloaded. If using Microsoft Windows, you will get a warning, select “More Info” and “Run Anyway”. It will install 
   the EdgeTX Flasher program and then open it (it may take a little while to install ~45s).

8. Plug in your radio (radio power should be off).

9. Select “Flash” from the menu on the left side of the screen.

10. Select “edgetx-firmware-2.4” from the option “Firmware Branch”

11. Select the latest firmware for the “Firmware Version”

12. Once metadata is downloaded, select the **CORRECT** radio type that you intend to flash.

13. Select “Write”.  You will see a message “Downloading bin” and then a “Flashing” popup with the flashing progress. Eventually, you will see “File downloaded successfully”. Ignore any error messages after that and click on "Close". 

14. After the flashing is complete, restart your radio and you should be greeted with "Welcome to EdgeTX."
 
> If you **did not** add the RADIO and MODELS folders to your SD card for conversion, when the radio starts with EdgeTX for the first time, you will get a STORAGE WARNING - Bad Radio Data-. Press the white circle or roller to bypass the warning. Then you will get another STORAGE WARNING - Storage Preparation. Press the white circle or roller again. Once the SD card is prepared, the calibration screen will appear. Calibrate your radio. 

> If you **did** add the RADIO and MODELS folders to your SD card for conversion you should see a message telling you that a SD conversion is necessary. Select the white circle and it will begin converting your models. Depending on your previous radio settings the calibration screen may appear, If so, Calibrate your radio.

### You are ready to start using EdgeTX!

_**Note 1: The radio software will default to mode 1 stick configuration. If you have a mode 2 radio, you will most likely see a “Throttle Warning”. To fix this, go into the radio settings and change to mode 2.**_



