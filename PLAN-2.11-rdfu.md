# Plan: 2.11 Windows Companion flashes with rdfu

**Delete this file, and the "DROP:" commit that adds it, before the branch
is merged.** It's here so the work can be picked up in a later session.

## Goal

Stop 2.11's Windows Companion depending on MSYS2 for DFU flashing.

MSYS2 is phasing out the `mingw64` environment (news, 2026-03-15). It
dropped `dfu-util` from mingw64 in Sept 2026. 2.11 currently works around
that by vendoring the last mingw64 dfu-util package in `.github/msys2/`, but
that package still needs mingw64 `libusb`, which is likely to go next.

`rdfu` is the command line tool from EdgeTX's `rs-dfu` project
(https://github.com/EdgeTX/rs-dfu, MIT). It's the same DFU engine that 2.12
and later link in as the `rs_dfu` library (#6521), so it's proven on the
handsets. It needs no libusb.

This is step 1 of a longer plan for 2.11, which may be supported for another
year or more:

1. **This branch:** rdfu replaces dfu-util on Windows.
2. **Later:** move the 2.11 Windows build from MSYS2 mingw64 to MSVC (clang++
   with the VS toolchain), as 2.12 did in #6476. Keep Qt 5.15.2, using aqt's
   `win64_msvc2019_64` build. Port #6603 (bundling the VC++ runtime).
   Watch for MSVC's 1 MB default stack (2.12 needed #6869/#6892) and the
   bitfield layout, since `-mno-ms-bitfields` is MinGW-only.

## Status (2026-10-07)

- Branch `pfeerick/2.11-rdfu`, based on `origin/2.11` (`110a92701e`).
- Compile-checked only: the changed Companion sources compile against Qt
  5.15.2 headers, and `cmake/FetchRdfu.cmake` was tested in script mode.
- Not yet built on CI. Not yet tested on Windows or with a radio.

## What the branch changes

Commit "feat(cpn): support rdfu as the DFU flashing tool":

- **`process_flash.{h,cpp}`:**
  - `isRdfuCommand()`: the tool is treated as rdfu when the executable name
    starts with `rdfu`, otherwise it's dfu-util.
  - rdfu prints its progress to stdout, redrawn in place with `\r`:
    `Erasing page N of M`, then `Flashing NN% [###]`, or `Reading NN%`.
    It's parsed by a regex over complete redraws only, since a redraw can be
    split across reads. The existing "Writing..." and "Reading..." strings
    are reused, so there are no new translations.
  - rdfu errors are `Error: ...` on stderr with exit code 1.
    `Error: No DFU device` shows the existing "radio not connected" tip.
- **`radiointerface.cpp`:**
  - `getRdfuArgs()` builds
    `write|read --vendor 0483 --product df11 --start-address 0x08000000 [--length <flash size>] <file>`.
    The saved dfu-util arguments (default `-a 0`) are ignored.
  - Writing a `.dfu` file with rdfu is refused with a message. rdfu only
    understands raw binaries and UF2, and would write the DfuSe container
    to flash as-is.
- **`burnconfigdialog.cpp` (Windows only):** the default tool is
  `<app dir>/rdfu/rdfu.exe`. A saved path that no longer exists also falls
  back to it, which covers the old bundled `dfu-util.exe` that the installer
  now deletes. Users who pointed Companion at their own dfu-util elsewhere
  keep it, since both tools are still supported.

Commit "chore(cpn): bundle rdfu instead of dfu-util on Windows":

- **`cmake/FetchRdfu.cmake`** (Windows only, included from
  `NativeTargets.cmake`):
  - Downloads `rdfu-Windows-x86_64.exe` v0.7.2, pinned by SHA-256. The hash
    matches GitHub's published asset digest.
  - Finds `vcruntime140.dll` in the VS redistributables via `vswhere`, taking
    the newest toolset.
  - Both can be overridden with `-DRDFU_EXECUTABLE=` / `-DRDFU_VCRUNTIME=`.
  - Without a vcruntime it only warns, and rdfu then needs the VC++ runtime
    installed. The bundled OpenSSL 1.1 DLLs already import VCRUNTIME140.dll,
    so 2.11 effectively needs it already.
- **`companion/src/CMakeLists.txt`:**
  - On Windows, libusb and dfu-util are no longer searched for or installed.
  - `rdfu.exe` + `vcruntime140.dll` are installed into `rdfu/` under the
    install directory. The subdirectory is deliberate: Companion loads DLLs
    from its own directory first, and an app-local vcruntime there could be
    picked up by other DLLs in Companion's process.
- **`companion.nsi.in`:** the installer deletes the old top-level
  `dfu-util.exe` / `libusb-1.0.dll`, and the uninstaller removes `rdfu\`.
- **CI (`win_cpn-64.yml`, `companion.yml`):** dropped the vendored dfu-util
  step, `mingw-w64-x86_64-libusb`, and `.github/msys2/` with its trigger
  path.

Linux and macOS are unchanged and still bundle dfu-util.

## Behaviour changes to note in the PR

- rdfu always leaves DFU mode after a write, so the radio reboots into the
  new firmware. The old dfu-util arguments had no `:leave`, so the radio
  stayed in DFU. This matches 2.12.
- `.dfu` files can no longer be flashed with the bundled tool.
- The "advanced" DFU arguments field has no effect with rdfu.
- The dialog title still says "DFU-UTIL Configuration". It was left alone to
  avoid breaking existing translations.

## How to test

1. Push to the fork, then run
   `gh workflow run companion.yml --repo pfeerick/edgetx --ref pfeerick/2.11-rdfu -f target=windows`.
   In the Windows job log, check for:
   - `-- rdfu: .../rdfu/rdfu.exe`
   - `-- rdfu VC++ runtime: C:/Program Files/Microsoft Visual Studio/2022/.../vcruntime140.dll`
     (if it says "not found" instead, check how vswhere is found under MSYS2)
   - `Installing: .../Release/rdfu/rdfu.exe` and `.../rdfu/vcruntime140.dll`
   - no dfu-util or libusb install lines
2. Also run `target=linux` and `target=macos` to confirm they still find
   dfu-util.
3. On real Windows:
   - Install over an existing 2.11 that has a saved dfu-util path (Radio >
     Configure Radio Communications..., click OK once on the old version).
     After the upgrade, check `dfu-util.exe` and `libusb-1.0.dll` are gone
     and the dialog shows `...\rdfu\rdfu.exe`.
   - Run `"<install dir>\rdfu\rdfu.exe" list` in `cmd`. There should be no
     missing-DLL errors. Also try it on a machine without the VC++ runtime,
     or with `vcruntime140.dll` renamed in System32, to prove the bundled
     copy is used.
   - Write firmware over DFU and check the progress bar moves through erase
     and write, and that the radio reboots.
   - Read firmware over DFU and check the file is the board's flash size.
   - With no radio connected, check the "radio not connected" tip appears.
   - Try a `.dfu` file and check the refusal message appears.
   - Uninstall and check `rdfu\` is removed.
4. Radios: at least one STM32F4 B&W radio, one F4 colour radio (2 MB
   flash), and the H7 boards 2.11 supports (FlySky PA01 / ST16), if
   available. Reading uses `Boards::getFlashSize()`, so check rdfu accepts
   that length on each.
5. **Driver.** rdfu talks to USB through WinUSB (nusb), not libusb. A radio
   bound to WinUSB with Zadig should work. Check what happens with a radio
   bound to libusbK or libusb-win32; dfu-util accepted those, rdfu may not.
   If it fails, the PR needs a note to re-run Zadig with WinUSB.

## Possible follow-ups

- Ask rs-dfu to build `rdfu.exe` with a static CRT
  (`-C target-feature=+crt-static`). That would remove the vcruntime
  dependency, and `FetchRdfu.cmake`'s vcruntime handling could be dropped.
- Optionally use rdfu on macOS and Linux too, for one flashing path on all
  platforms. Not needed for the MSYS2 problem.
- The MSVC switch (step 2 above).
