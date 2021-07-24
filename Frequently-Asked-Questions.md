

**Will development for OpenTX stop?**

No, there are no plans to stop developing OpenTX. Below is a quote by Kilrah from the OpenTX development team 
> “The idea is not to stop developing OpenTX, but also not to risk breaking stuff that people use while introducing things most don't care about, yet not to slow down experimentation. There is quite a lot of legacy and large installed userbase we don't want to disappoint.
>  The plan is that EdgeTX will be where "experiments" with various stuff happen, and once done and validated if deemed worthy they get taken back into OpenTX with any necessary adjustments done to ensure OpenTX remains well stable. We'll see how this goes.”

***

**How do I install EdgeTX firmware?**

Detailed instructions for flashing the EdgeTX firmware can be found here: 
[https://github.com/EdgeTX/edgetx.github.io/wiki/EdgeTX-Installation-Guide](https://github.com/EdgeTX/edgetx.github.io/wiki/EdgeTX-Installation-Guide)

**Will the EdgeTX releases be just beta releases for Open TX or will they be tested and stable for everyday use?**

The EdgeTX releases will be tested and considered stable for everyday use. However, as these releases will occur at regular intervals, they may not reach the level of maturity that OpenTX main releases achieve. We will try our very best to make sure that the software you install on your TX is working and stable. However, issues could get missed in testing and make it into the release. We hope that this is a rarity.


**What features / changes are included in EdgeTX that are not included in the latest OpenTX Release?**

To see the list of improvments as well as limitations of EdgeTX, please review the [release notes for the 2.4.0 release.](https://github.com/EdgeTX/edgetx/releases/tag/v2.4.0)


**Will I still be able to use OpenTX Companion to manage my models with EdgeTX?**

Yes, you will still be able to manage your models with OpenTX Companion. The new EdgeTX flasher will only be for downloading the firmware and flashing your radio.

 _Please note- OpenTX Companion is currently NOT compatible with EdgeTX - we are working on that. However, you can import and convert your existing models to use with EdgeTX. See the [installation guide](https://github.com/EdgeTX/edgetx.github.io/wiki/EdgeTX-Installation-Guide) for more information._

**What radio-transmitters will be supported?**
* X9Lite 
* QX7 (non-Access)
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
* T18

Basically, everything that is supported in OTX 2.3, minus 9X / 9XR-pro.

**Does EdgeTX support the Flysky Nirvana NV14?**

EdgeTX does not currently support the Flysky Nirvana NV14. However, we are actively working to support it. We are hopeful to have a usable version for testing in mid to late August with the intent to have it supported in our next release.
