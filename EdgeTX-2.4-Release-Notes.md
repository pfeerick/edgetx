# -Draft-

We are happy to bring you the first release of EdgeTX – the cutting edge open source firmware for your R/C radio. EdgeTX is a fork of OpenTX 2.3.11, so it contains all the current features of OTX2.3.11 as well and some additional features that were slated for OTX 2.3.12 including:
* Add support for RadioMaster T8
* Add EXT1 and EXT2 to calibration screen
* Add support for Jumper T-Lite
* Fix multi per channel failsafe
* Add telemetry sources for inputs
* Add GHST GPS Telemetry Sensors
* M-Link telemetry
* Multi module config support
* New Lua function getAvailableMemory
* Improved sync for CRSF Shot
* R9M Flex support broken 2.3.12N495
* Stop mixer scheduler when stopping module
* Set correct channel count for Ghost modules
* Several other small fixes
Additionally, EdgeTX adds the several other enhancements and new features:
* Added touchscreen support for touchscreen equipped radios (including T18)
* Overhauled user interface to better support touchscreen use
* Fix mixer scheduler 33Hz only issue
* Remove legacy ZCHAR
* Radios with internal GPS will display GPS icon and sat number in top bar
* Added OneBit mode to HARDWARE menu in order fix inverter bug in x9D+ and X7
* Raised maximum mixer scheduler period to 50ms (20Hz)
* Added key handling for lua scripts
* Added additional graphics plotting functions
* Make timers conditional on switch (if set)
* Added auto-switch / auto-source for color lcd radios
* Added automated model conversion for models from OpenTX on startup.
* Added many small “under the hood” enhancements and fixes.

### Supported radios
* X9Lite
* QX7 (non-Access)
* T12
* TX12
* T8
* Tlite
* XLite
* X9D
* X9DP
* X9DP2019
* X10
* T16
* TX16S
* X12S
* T18

### Limitations:
Please note the following limitations. At the time of release:
* You are not (yet!) able to import or edit EdgeTX models using OpenTX Companion (but firmware flashing with OpenTX Companion is still possible). Your OpenTX models and settings will be automatically converted by EdgeTX, and changes to model and radio settings will need be done on the radio until the Companion software is updated to suit.
* Panel / widget will not be preserved in the conversion from OTX 2.3.x => ETX 2.4.0 due to incompatibility in how the settings sere stored (OTX2.3 does not include the type of the options in the binary file). This should be a one-time issue.
* Support for FlySky Nirvana NV14 is not yet available.
* Users are not able to select a theme or modify the default theme color scheme.

### Download Links:
EdgeTX Flasher: [https://github.com/EdgeTX/flasher/releases](https://github.com/EdgeTX/flasher/releases)

EdgeTX SD Card: [https://github.com/EdgeTX/edgetx-sdcard/releases](https://github.com/EdgeTX/edgetx-sdcard/releases)

### References:
EdgeTX website - https://edge-tx.org/

Installation Guide - https://github.com/EdgeTX/edgetx.github.io/wiki/EdgeTX-Installation-Guide

Installation Video: - https://www.youtube.com/watch?v=Y9OvW9XCjOs

Which SD Card Zip should I use? - [https://github.com/EdgeTX/edgetx-sdcard](https://github.com/EdgeTX/edgetx-sdcard)

Reporting Issues / Requesting features: [https://github.com/EdgeTX/edgetx/issues/new/choose](https://github.com/EdgeTX/edgetx/issues/new/choose)

Frequently Asked Questions - https://github.com/EdgeTX/edgetx.github.io/wiki/Frequently-Asked-Questions

Community Guidelines - https://github.com/EdgeTX/edgetx.github.io/wiki/Community-Guidlines

### Thanks:
Special thanks to all those that contributed to making this release possible! Without the support and commitment from the development team and the community, this release would not have been possible.
Here are the top contributors to this release: **@raphaelcoeffic, @richardclli, @CoderElectronics, @olliw42, @pfeerick, @yaapu, @rotorman, @hydra, @MRC3742, @gretel, @eshifri, @norulers.**
A big thanks goes also to the OpenTX team and previous developers, on which EdgeTX is based upon.

### Keep in touch!
Discord - https://discord.gg/wF9wUKnZ6H

Facebook - https://www.facebook.com/groups/edgetx

