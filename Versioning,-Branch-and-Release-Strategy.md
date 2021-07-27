# DRAFT

## Versioning Scheme

EdgeTX uses a 3 stage versioning scheme, the major version number is listed first, followed by the minor version number, and lastly the bugfix version number (major.minor.bugfix).

*For example:*

v2.4.0 - Current latest release

v2.4.1 - 1st bugfix release of v2.4 (released as needed)

v2.4.2 - 2nd bugfix release of v2.4 (released as needed) 

v2.5.0 - Minor release (planned for September 2021) - new features, enhancements and bug fixes.

v2.5.1 - 1st bugfix release of v2.5 (released as needed)

v2.6.0 - Minor release (planned for December 2021) - new features, enhancements and bug fixes.


## Branch Strategy

**edgetx-firmware-main** - This is the main branch. All code changes are added to this branch once the changes have been tested and reviewed by another developer. If you download firmware from this branch, you will have all code changes that have been checked in by the developers. It is important to note that full system testing has NOT been completed and only feature/function-specific testing had been completed. _This branch is only visible with the Flasher in advanced mode._

**edgetx-firmware-merge** - This is a copy of the main branch that has specific changes added to it for testing purposes. Once the changes have been tested and are deemed safe, they are then merged into the main branch. Normally, the developers ask a specific testers user group to evaluate a build from the merge branch, before it is merged to the main branch. If you are interested in helping out with testing, come and join EdgeTX Discord in #tests channel. _This branch is only visible with the Flasher in advanced mode._

**edgetx-firmware-nightly** - This is a "snapshot" of the main branch, created every night, normally after all changes have been checked in for a certain day. Like the main branch, only feature/function-specific testing has been completed. However, nightly builds may be considered a bit more stable as they contain all the changes from a certain day rolled up together and tested.

**edgetx-firmware-vX.Y.Z** - This is the release branch. Once all features and fixes that have been planned for a release are finished and initial testing has been completed, the release will be detached from the main branch. Once that occurs, only bug fixes are applied to the release branch. Further feature development and enhancments will continue on the main branch, but not on the release branch. Any bug fixes that are made to the release branch are also propagated to the main branch.

**edgetx-firmware-XXXX** - This is a feature-specific branch of the main branch that has been created by a developer to develop and test on. Normally end-users should not install these branches unless specifically asked by a developer for testing purposes. _These branches are only visible with the Flasher in advanced mode._


## Release Strategy

EdgeTX plans to have 4 minor releases per year. Bug fix releases will occur as needed between minor releases. Major releases are not currently planned at particular time intervals and will occur based on the future roadmap.

In the case of major and minor releases (see versioning scheme above), once the release branch has been created and stabilized, a release candidate (RC) will be made from the release branch. The RC is used for the final testing and published for evaluation and feedback. Generally speaking, a release candidate is considered higher quality than a BETA release but that has not yet matured to a release.

Depending on the outcome of the testing, as well as feedback received from the public, additional bug fixes may be made, resulting in further RC versions being published. If no major bugs were found, then the RC will become the next release version.

In the case of bug fix releases, as bugs are found in the release, they will be fixed in both the release branch and the main branch. Once several bugs or a critical bug has been fixed, a bug fix release will be created for the release branch. This new bug release will increment the version in the third decimal place, for example: v2.4.1 is a bug fix release for v2.4.0.

Generally, bug fix releases are not preceded with release candidates, since they normally do not contain feature changes to the software and only are tested by the developers and the test team.