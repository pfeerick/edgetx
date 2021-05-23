

**Will development for OpenTX stop?**

No, there are no plans to stop developing OpenTX. Below is a quote by Kilrah from the OpenTX development team 
> “The idea is not to stop developing OpenTX, but also not to risk breaking stuff that people use while introducing things most don't care about, yet not to slow down experimentation. There is quite a lot of legacy and large installed userbase we don't want to disappoint.
>  The plan is that EdgeTX will be where "experiments" with various stuff happen, and once done and validated if deemed worthy they get taken back into OpenTX with any necessary adjustments done to ensure OpenTX remains well stable. We'll see how this goes.”

***

**When will EdgeTX be released to the public?**

The planned release date is 21 June 2021.  However, if you want to try it out sooner or help with testing you are free to install one of the automated builds from the Github page. We will inform the community once the release candidate has been built.

**Where can I download the EdgeTX firmware to test it out?**

You can download the latest automated build from the actions artifacts on our Github Page here: [https://github.com/EdgeTX/edgetx/actions](https://github.com/EdgeTX/edgetx/actions) Please note that a github account is required.

If you are not familiar with Github or do not have a Github account, you can also download the EdgeTX Firmware from our Google Drive account here:
[EdgeTX Google Drive](https://drive.google.com/drive/folders/1vcmth59lCe7zaiSVox-AK3ZCOwle6g3M?usp=sharing). Additionally, **detailed flashing instructions are also included in the Google drive link above.**

**Will the EdgeTX releases be just beta releases for Open TX or will they be tested and stable for everyday use?**

The EdgeTX releases will be tested and considered stable for everyday use. However, as these releases will occur at regular intervals, they may not reach the level of maturity that OpenTX main releases achieve. We will try our very best to make sure that the software you install on your TX is working and stable. However, issues could get missed in testing and make it into the release. We hope that this is a rarity.


**What features / changes will be included in EdgeTX that are not included in the latest OpenTX Release (version 2.3.11)?**

The main focus for new features in this release are:
* Touch screen support based on the new color LCD UI.
* Model / Radio data storage being moved from binary to a more flexible format (YAML).
* A simple flasher to ease getting new firmware into radios.
* Several other small enhancements & fixes will be listed in detail in the release notes.


**Will I still be able to use OpenTX Companion to manage my models with EdgeTX?**

Yes, you will still be able to manage your models with OpenTX Companion. The new EdgeTX flasher will only be for downloading the firmware and flashing your radio. 

**What radio-transmitters will be supported?**
* X9Lite 
* QX7
* T12 
* TX12
* T8
* Tlite
* XLite
* X9DP 
* X9DP2019 
* X10
* T16
* TX16S 
* X12S
* T18 - planned, but not confirmed at this time.

Basically, everything that is supported in OTX 2.3, minus 9X / 9XR-pro.

