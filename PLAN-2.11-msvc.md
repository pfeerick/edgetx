# Plan: build 2.11 Windows Companion with MSVC

**Delete this file, and every "DROP:" commit, before the branch is merged.**
It's here so the work can be picked up in a later session.

## Goal

Move 2.11's Windows Companion build off MSYS2's `mingw64` environment, which
MSYS2 is phasing out (news, 2026-03-15). Use the same toolchain as 2.12 and
later: LLVM `clang++` targeting the MSVC ABI, with the Visual Studio headers
and libraries (#6476). 2.11 may be supported for another year or more.

Breakage is already happening:
- dfu-util was dropped from mingw64 (Sept 2026). That's handled by the rdfu
  branch this one is stacked on.
- On 2026-10-07, upstream 2.11's Windows job failed at `makensis` with
  `Plugin not found, cannot call StartMenu::Init`. mingw64's NSIS package
  is broken. This branch installs NSIS with `repolevedavaj/install-nsis`
  instead, as 2.12 does.

## Branch layout

- Branch `pfeerick/2.11-msvc` is stacked on the rdfu branch
  (`pfeerick/2.11-rdfu`, fork draft PR #31), without that branch's `DROP:`
  planning commit. Rebase onto `2.11` once the rdfu change has merged.
- The commits are:
  1. A port of #6476's source fixes.
  2. The MSVC build itself (CMake, NSIS, CI).
  3. `DROP:` limit the simulator plugins built on Windows CI.
  4. `DROP:` this file.

## Decisions

- **Qt stays at 5.15.2**, using aqt's `win64_msvc2019_64` build. VS2022 is
  binary compatible with it. A Qt 6 move is out of scope.
- **Compiler: `clang++`, not `cl.exe`.** It's the same as 2.12, and it keeps
  the GNU-style flags 2.11's CMake uses. Note that clang targeting MSVC
  defines `_MSC_VER` and not `__GNUC__`, so every legacy `_MSC_VER` branch
  in the code becomes active. #6476 removed the broken ones in
  `yaml_node.h` and `zone.h`. The remaining ones in `helpers.cpp`,
  `logsdialog.cpp`, `simulatorinterface.cpp`, `bluetooth.cpp`,
  `simpgmspace.cpp`, `simufatfs.cpp` and `process_flash.cpp` exist in 2.12
  too, except `simufatfs.cpp`. 2.12 rewrote that file (#6454), so 2.11's
  old MSVC path there (`MSVC_BUILD`, `thirdparty/windows/dirent`) is
  untested.
- **pthreads: pthreads4w from vcpkg** (`pthreads:x64-windows`, which
  produces `pthreadVC3.dll`, installed next to `companion.exe`). 2.11's
  simulator uses pthreads in `rtos.h`, `simpgmspace.cpp`, `simudisk.cpp` and
  `simueeprom.cpp`. 2.12 replaced pthreads with C++ threads (#6449), but
  that change sits on the `radio/src/os/` task/timer layer, which 2.11
  doesn't have. That's too large for a maintenance branch.
  `cmake/NativeTargets.cmake` finds it via `CMAKE_PREFIX_PATH` and adds it
  to `WIN_INCLUDE_DIRS` / `WIN_LINK_LIBRARIES`.
- **SDL2:** the official `SDL2-devel-<ver>-VC.zip`, as 2.12 does. Its config
  sets `SDL2_LIBRARIES` (which includes SDL2main) and `SDL2_INCLUDE_DIRS`, as
  2.11's CMake expects. `SDL2.dll` is located from the `SDL2::SDL2` imported
  target, because the old `find_file` hints only cover MSYS2.
- **VC++ runtime:** ported from #6603, with two changes:
  - `cmake/FindMSVCRedist.cmake` globs
    `$VCINSTALLDIR/Redist/MSVC/*/vc_redist.x64.exe`. 2.12's version passes
    globs to `find_program` `PATHS`, which don't expand there.
  - The NSIS script uses NSIS's bundled `FileFunc.nsh`/`WordFunc.nsh`
    instead of vendored copies, and reads the runtime version from both
    registry views (the installer is 32-bit). `vc_redist` is referenced from
    the build tree, not installed into the app directory.
- **Stack size: `/STACK:8388608`** on `companion.exe` and `simulator.exe`.
  MSVC defaults to 1 MB, half of MinGW's default, and 2.12 hit stack
  overflows creating and pasting models (#6869, #6892). Threads created
  without an explicit size, including pthreads4w's, use the executable's
  default.
- **windeployqt:** use `--release --no-compiler-runtime` under MSVC. The
  MinGW `--debug` workaround would deploy debug Qt DLLs. The options are
  now passed as a real list with `VERBATIM`; they used to be one quoted
  string, which Ninja/`cmd.exe` wouldn't split.
- **Simulator plugins** keep the `lib` prefix (#6476), so DLL names match the
  MinGW builds and upgrades overwrite them.
- **Upgrades:** the installer deletes the leftover MinGW runtime DLLs
  (`libgcc_s_seh-1`, `libstdc++-6`, `libwinpthread-1`).

## Status (2026-10-07)

- First cut, not yet built on Windows.
- The ported radio sources were checked with ARM firmware builds (X7, X9D+,
  TX16S).
- Iterating on fork CI with the `DROP:` plugin limiter
  (`EDGETX_SIMU_PLUGINS` in the workflow). Run it with
  `gh workflow run companion.yml --repo pfeerick/edgetx --ref pfeerick/2.11-msvc -f target=windows`.

## Known risks / things to check in CI logs

- `SDL2_LIBRARIES` includes `SDL2::SDL2main`. Check Companion still links
  Qt's WinMain, not SDL's, and that no file including `SDL.h` gets
  `main` renamed.
- pthreads4w and `struct timespec`: the UCRT defines it, and pthreads4w
  should detect that. If the compiler reports a redefinition, add
  `-DHAVE_STRUCT_TIMESPEC` for `WIN32 AND NOT MINGW`.
- **Bitfield layout:** `-mno-ms-bitfields` is MinGW-only, so packed
  bitfield structs get Microsoft's layout. 2.12 works with that, but watch
  for size `static_assert`s, and test the YAML round-trip.
- The MSVC_BUILD code in `simufatfs.cpp` (see above).
- The `cmd.exe` pipe in `AddHardwareDefTarget` (`grep | sort` removed, as in
  #6476).

## Windows hardware tests (once CI is green)

- Fresh install, and install over a MinGW-built 2.11.x. Check the VC++
  runtime install, the leftover MinGW DLLs being removed, and plugins being
  overwritten.
- Open, edit and save models and radio settings (YAML round-trip) for a B&W
  and a colour radio. Create a new model, and copy/paste models (the stack
  issue).
- Simulator for a B&W radio (x9d/x7), a colour radio (tx16s), and a
  portrait one (nv14). Check audio (SDL), the SD card image (simufatfs),
  and Lua.
- Flashing via rdfu (see the rdfu branch's checklist).
- Uninstall cleans up, including `pthreadVC3.dll` and `rdfu\`.

## Follow-ups

- Once MSVC is in, rdfu's app-local `vcruntime140.dll` (`FetchRdfu.cmake`)
  is redundant, because the installer installs the VC++ runtime. It could
  be dropped.
- #7245 (NSIS `$PROGRAMFILES` → `$PROGRAMFILES64`) applies to 2.11 too.
  It's independent of this change.
- Update the docs and `tools/` scripts for building 2.11 on Windows, which
  are MSYS2-based.
