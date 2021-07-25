# DRAFT

## Versioning Scheme

Although EdgeTX is starting with a versioning scheme inline with OpenTX, we have decided to transition to a more industry standard versioning scheme going forward. In this scheme, the major version will be listed first, followed by the minor version, followed by the bugfix version. 

*For example:*

v2.4.0 - Current version

v2.4.1 - 1st bugfix release of v2.4 (released as needed)

v2.4.2 - 2nd bugfix release of v2.4 (released as needed) 

v2.5.0 - Minor release (planned for September) - new features, enhancements and bug fixes.

v2.5.1 - 1st bugfix release of v2.5 (released as needed) v2.6.0 - Minor release (planned for December) - new features, enhancements and bug fixes.


## Branch Strategy

**edgetx-firmware-main** - This is the main branch, also known and the "Trunk". All code changes are added to this branch once the change has been reviewed by another developer and has been tested. If you download firmware from this branch, you will have all code changes that have been checked in by the developers. It is important to note that full system testing has NOT been completed and only feature/function-specific testing had been completed. _This branch is only visible with the Flasher in advaced mode._

**edgetx-firmware-merge** - This is a copy of the trunk that has specific changes added to it for testing purposes. Once the changes have been tested and are deemed safe, they are then merged into the trunk. Normally, testers or certain users may get asked to test a build from the merge branch if they are helping a developer test a new feature or fix before it is merged to the trunk. _This branch is only visible with the Flasher in advaced mode._

**edgetx-firmware-nightly** - This is a "snapshot" of the trunk created every night, normally after all changes have been checked in for a certain day. Like the trunk, only feature/function-specific testing had been completed. However, nightly builds may be considered a bit more stable as they contain all the changes from a certain day rolled up together and tested.

**edgetx-firmware-v2.x.x** - This is the release branch. Once all features and fixes that have been planned for a release are complete and initial testing has been completed, the release branch will be branched from the trunk. Once that occurs, the only changes that will be made to this branch are for bug fixes. Further feature development and enhancments will be added to the trunk, not the release branch.  Any bug fixes that are made to the release branch are also made in the trunk to keep them aligned. 

**edgetx-firmware-XXXX** - This is a feature-specific branch of the trunk that has been created by a developer to develop and test on. Normally users should not install these branches unless specifically asked by a developer for testing purposes. _These branches are only visible with the Flasher in advaced mode._


## Release Strategy

EdgeTX will have 4 minor releases per year.  Bug fix releases will occur as needed between minor releases.  Major releases are not currently planned at particular time intervals and will occur based on the future roadmap. 

In the case of major and minor releases (see versioning scheme above), once the release branch has been created and stabilized, a release candidate (RC) release will be made from the release branch. The RC is used for the final regression testing by the testing team as well as published to the public for evaluation and feedback. Generally speaking, a release candidate is considered higher quality than a BETA release but has not yet achieved final release quality. 

Depending on the outcome of the regression testing as well as feedback received from the public, additional bug fixes may be made, resulting in another RC being released. If no bugs were found, then the RC release will become the final release.

In the case of bug fix releases, as bugs are found in the release they will be fixed in both the release branch and trunk. Once several bugs have been fixed or a critical bug has been fixed, a bug fix release will be created from the release branch. This new bug release will increment the version in the third decimal place, for example: v2.4.x. 

Generally, release candidates are not created for bug fix releases since they normally do not contain significant changes to the software and only are tested by the developers and test team.