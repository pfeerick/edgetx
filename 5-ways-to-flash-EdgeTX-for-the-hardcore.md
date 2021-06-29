### A short summary of 5 ways to flash EdgeTX, first when looking from the point of flashing application used:
1. **Use EdgeTX flasher** https://github.com/EdgeTX/flasher/releases
   Benefit: easy to use, incorporates EdgeTX SD card creation. Can also be used when bootloader binary on radio is screwed up.
   Downside: Device-Firmware-Upgrade (DFU) mode only, under Windows need DFU drivers explicitly installed first.

2. **Use OpenTX Companion** https://www.open-tx.org/downloads
   Benefit: can use DFU or bootloader mode. Flashing in DFU mode can also fix a "bricked" radio when bootloader does not run.
   Downside: need to untick "Check hardware compatibility" when flashing EdgeTX for now

3. **Copy EdgeTX binary to SD card FIRMWARE subfolder** and use running OpenTX to flash the EdgeTX bootloader and then use EdgeTX bootloader to flash EdgeTX binary
   Benefit: one of the most robust ways to flash, does not need any special drivers or software on your PC/Mac.
   Downside: two step process and your radio needs to at least run OpenTX. This method cannot be used to "un-brick" your radio if bootloader is damaged.

4. **Use STM32CubeProgrammer** in DFU mode.
   Benefit: comes with appropriate DFU drivers, very reliable, the "official" way of flashing STM32 µC.
   Downside: more options and buttons to click than with other methods and thus likely not suitable for novices or not technically inclined.

5. **Use hardware programmer/debugge**r to flash the STM32 chip via Serial-Wire-Debug (SWD) header.
   Benefit: can not only program, but also in-circuit-debug, no DFU drivers required.
   Downside: need to open the radio (void possibly warranty), hardware hook-up to SWD pins & knowledge of the appropriate flasher software required (e.g. STM32CubeProgrammer, Segger J-Flash).

### Here the EdgeTX flashing options, when looked from the point of flashing modes and their benefits and downsides:
1. **Device-Firmware-Upgrade (DFU) mode**
  How: attach the radio to PC/Mac while powered off.
  Benefit: can recover broken bootloader, flashes bootloader and firmware in one go.
  Downside: PC/Mac needs DFU drivers (Windows) or dfu-utility (Mac, Linux). Flashing is slow (bigger color radios can take up to 2 minutes to flash).

2. **Bootloader mode with PC/Mac**
  How: while not attached to PC/Mac, pull trims T1 and T4 together and power on the radio. Now attach the USB cable to PC/Mac and use Companion.
  Benefit: relatively quick flashing.
  Downside: can only flash the firmware, not the bootloader itself.

3. **Bootloader mode, stand-alone**
  How: copy the binary to SD card FIRMWARE subfolder. While not attached to PC/Mac, pull trims T1 and T4 together and power on the radio. Select the firmware to flash and long press Enter/Roller to start flashing the firmware.
  Benefit: relatively quick and very reliable flashing w/o additional software or drivers involved.
  Downside: can only flash the firmware, not the bootloader itself.

4. **Flashing (bootloader) from running OpenTX/EdgeTX**
  How: copy the binary to SD card FIRMWARE subfolder. Power up the radio normally and navigate into SYSTEM -> SD card -> FIRMWARE and long press on the binary.
  Benefit: very robust and easy way to flash EdgeTX bl, no special software or drivers needed.
  Downside: can only flash the bootloader, not EdgeTX firmware in one go.

5. **Flashing via Serial-Wire-Debug (SWD)**
  How: solder header or wires to internal SWD header and attach a hw-programmer/debugger such as ST-LinkV2 or Segger J-Link/J-Trace.
  Benefit: the fastest way of flashing (with 50 MHz J-Link/J-Trace, roughly 3 sec. on color radio), flashes bootloader and firmware in one go, can thus also unbrick radio, no DFU drivers required.
  Downside: viable option only for developers with appropriate hardware and knowledge.

_Special thanks to Risto for the detailed and informative contribution._