<p align="center">
<a href="url"><img src="https://github.com/EdgeTX/edgetx.github.io/blob/master/images/edgetx-v2.png" align="center" height="150" width="150" ></a>

# How to install EdgeTX for the first time (pre-release)
As of 27.May.2021

### Warning: the EdgeTX firmware is still in a pre-release state and should only be used for testing purposes. It has not yet been thoroughly tested and may contain issues.  Use at your own risk!
_**Note 1: Before you update to EdgeTX, it is highly recommended that you backup your current firmware and SD card contents in case you need to roll back to your previous version for some reason.**_

_**Note 2: This method of downloading and installing the firmware will only be used until the new firmware flasher is finished being developed.**_

1. Download and extract the updated SD card contents for your radio type to your computer. The new SD card contents can be found here: https://github.com/EdgeTX/edgetx-sdcard/releases

_**Note: For more information about which folder to use for your radio, look at the readme page here: https://github.com/EdgeTX/edgetx-sdcard#readme**_

2. Delete everything from your SD card and copy the contents of the unzipped folder to your blank SD card.

3. Download the current EdgeTX Firmware.  You can download the most recent build directly from Github - https://github.com/EdgeTX/edgetx/actions (Github account required) or you can download the daily night-build directly from here: [Firmware](https://drive.google.com/drive/folders/1vcmth59lCe7zaiSVox-AK3ZCOwle6g3M?usp=sharing)

4. Unzip the file and save the correct .bin file (same name as your radio type) to your computer. 

5. Flash your radio with OpenTX Companion.  The radio should not be powered up and make sure the “Check Hardware Compatibility” box is not checked in the “Flash Firmware” dialog box. Flashing progress will be slow – that is normal. 
<p align="center">
<a href="url"><img src="https://github.com/EdgeTX/edgetx.github.io/blob/master/images/Picture3.png" align="center" height="363" width="602" ></a>


6. After the flashing is complete, restart that radio and it should be updated to EdgeTX. When the radio restarts, you will get a STORAGE WARNING - Bad Radio Data-. Press the white circle to bypass the warning. Then you will get another STORAGE WARNING - Storage Preparation. Press the white circle again and once the SD card is prepared, the calibration screen will appear.  Calibrate your radio and then you are ready to use!

_**Note 1: The radio software will default to mode 1, so if you have a mode 2 Radio, you will most likely see a “Throttle Warning”.  To fix this, go into the radio settings and change to mode 2.**_

_**Note 2: Flashing with OpenTX companion is only needed for the first time to update the bootloader.  After this has been done one time, you can perform further updates to the firmware without OpenTX Companion by just installing the new firmware .bin file to the firmware folder on the SD card and updating via bootloader mode on the radio.**_

### **For radios without SD Cards for storage use the following steps:**

1. Download firmware to your computer as described above (steps 3-4 above)
2. Flash radio with openTX Companion as described in Step 5 (- this will update the bootloader).
3. Start Radio normally and format the EEPROM when prompted
4. Start the Radio in Bootloader Mode using the “three finger boogie*”.
5. Flash the radio again with openTX companion (this will update the Firmware).

* ”three  finger boogie” = holding  trim switches T4 and T1 to center while pushing the power button on.
