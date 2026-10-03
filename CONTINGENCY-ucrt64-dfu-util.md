# Contingency: 2.11 Windows Companion with ucrt64 dfu-util

**Drop this file (and the commit that adds it) before adopting the branch.**
It exists so the branch explains itself if it's picked up months later.

## Status (2026-10-03)

- Parked, unmerged. Not yet built on CI or tested on Windows.
- The adoptable change is the commit "chore(ci): build 2.11 Windows Companion
  with ucrt64 dfu-util". Its message explains the change and the
  C-runtime/DLL reasoning in full; read it first.
- It was based on the 2.11.8 tracking branch tip (`4c03c7301c`, PR #7816),
  which already contains the alternative fix it replaces: vendoring the last
  mingw64 dfu-util package in `.github/msys2/` (`223cf99960`).

## When to use it

Only if the 2.11 Windows Companion build breaks again because MSYS2 dropped
another mingw64 package. The most likely candidate is `mingw-w64-x86_64-libusb`.
The vendored dfu-util depends on it, so `pacman -U` would then fail with
"unable to satisfy dependency".

Background: MSYS2 is winding down the mingw64 (msvcrt) environment in a
series of "drop mingw64 ... part n" PRs, several a day in late September
2026. dfu-util went in msys2/MINGW-packages #31695 (2026-09-17), which
targeted libusb-using tools. 2.12 and `main` aren't affected: they use
`rs_dfu` (#6521) and build Companion with MSVC (#6476).

If MSYS2 drops core pieces too (toolchain, SDL2, nsis, the pillow
stack), this branch isn't enough. The whole 2.11 Windows build would need to
move to ucrt64, including replacing Qt's msvcrt `mingw81_64` build with
MSYS2's ucrt64 Qt 5 packages. Otherwise Companion would load two C runtimes.

## How to adopt

1. Rebase onto the then-current `2.11`, taking only the change commit:
   `git rebase --onto origin/2.11 <commit before the change> <branch>`, then
   drop this notes commit. After #7816 is rebase-merged, the
   `.github/msys2/` commit exists on 2.11 under a new SHA, so expect the
   rebase to apply cleanly. If it doesn't, the change is: remove
   `.github/msys2/` and its trigger path in `companion.yml`.
2. Run the Windows Companion workflow (push to a fork, then
   `gh workflow run companion.yml --ref <branch> -f target=all` there).
   In the Windows job log, check for:
   - `pacman` installing `mingw-w64-ucrt-x86_64-dfu-util` (plus ucrt64 libusb
     and libwinpthread) from the MINGW64 shell
   - `Found LIBUSB1: .../msys64/ucrt64/lib`
   - `Found DFU_UTIL: .../msys64/ucrt64/bin/dfu-util.exe`
   - `Installing: .../Release/dfu-util/` for `dfu-util.exe`,
     `libusb-1.0.dll` and `libwinpthread-1.dll`
3. Test the installer on a real Windows machine:
   - Install over an existing 2.11 that has a saved dfu-util path (Radio >
     Configure Radio Communications..., click OK once on the old version).
   - Check the old top-level `dfu-util.exe` and `libusb-1.0.dll` are gone and
     the dialog shows `...\dfu-util\dfu-util.exe`.
   - In `cmd`, run `"<install dir>\dfu-util\dfu-util.exe" -l`. There should
     be no missing-DLL or "entry point not found" errors.
   - Flash, or read, a radio over DFU from Companion.
   - Uninstall and check the install directory is removed.

## Evidence gathered (2026-10-03)

| Package (MSYS2 ucrt64) | Version | SHA-256 |
|---|---|---|
| mingw-w64-ucrt-x86_64-dfu-util | 0.11-2 | `62e250375374fd272ef49bc5704e0d63f490e4b6d54b3f16f121fb190b53721d` |
| mingw-w64-ucrt-x86_64-libusb | 1.0.30-1 | `20106e6ff31b4581bb111b28a68b6b79a50dc3e5fc3033944e1c4b3ff8cd812e` |
| mingw-w64-ucrt-x86_64-libwinpthread | 14.0.0.r426.g4564ee4b5-1 | `f8de8153bbc0e47ba244a423c12c426fa1d9f56395117ecaa34c6ad6ebed6ca3` |

PE imports:
- **`dfu-util.exe`:** KERNEL32, `api-ms-win-crt-*` (UCRT), `libusb-1.0.dll`,
  and `libwinpthread-1.dll` (only `nanosleep64`).
- **`libusb-1.0.dll` and `libwinpthread-1.dll`:** KERNEL32 and
  `api-ms-win-crt-*` only.

Checks against the shipped 2.11.7 installer:
- `windeployqt` puts msvcrt `libgcc_s_seh-1.dll`, `libstdc++-6.dll` and
  `libwinpthread-1.dll` next to `companion.exe`. Windows searches the
  executable's own directory first, which is why dfu-util gets its own
  subdirectory.
- No Companion binary imports libusb. dfu-util is the only user, and it runs
  as a separate process.
- The bundled OpenSSL 1.1 DLLs already import `api-ms-win-crt-*`, so 2.11
  already requires the UCRT. This adds no new OS requirement.

How CMake finds the packages: the `Libusb1_ROOT` and `Dfuutil_ROOT`
environment variables (honoured under policy CMP0074, set by
`cmake_minimum_required(3.13)`).
- They are searched before the system prefix and PATH, so a mingw64 libusb
  can't win even if it's present.
- The native sub-build inherits them.
- This was checked with a mock superbuild with decoy mingw64 copies, under
  CMake 4.3.3.

Behaviour change to be aware of: if a user saved a bare `dfu-util` (meaning
"find it on PATH"), it fails the new "saved path exists" check, so Companion
switches it to the bundled copy.
