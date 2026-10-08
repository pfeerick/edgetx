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
  5. Fixes found while iterating on CI, listed under Status below.
  6. `DROP:` notes updates, and the commit that turns the limiter off again.
- When tidying for review, drop every `DROP:` commit and, if wanted, move
  the fix commits up next to commit 2. Use a non-interactive rebase.

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
  too, except `simufatfs.cpp`. 2.11's old MSVC path in that file couldn't
  work: `windows.h` and FatFs's `ff.h` define clashing `DWORD`/`WCHAR`. So
  #6454 (the `std::filesystem` rewrite 2.12 ships) is ported instead.
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

## Status (2026-10-08)

**Windows CI is green with the plugin limiter on** (x7, x9d, tx16s, nv14),
in fork run 37704472973. The installer artifact was checked:
- `companion-windows-2.11.8.exe` is 43.5 MB.
- It contains release Qt DLLs and no MinGW DLLs. It has `pthreadVC3.dll`,
  `SDL2.dll`, the OpenSSL 1.1 DLLs, `lib`-prefixed plugins, `rdfu\` with its
  `vcruntime140.dll`, and `vc_redist.x64.exe` (extracted to `$TEMP` only).
- The PE headers show an 8 MB stack on `companion.exe`/`simulator.exe`.
  Imports are `MSVCP140`/`VCRUNTIME140`, Qt, SDL2, and `pthreadVC3`
  (plugins only).

Fixes found while iterating, each its own commit:
- The superbuild's `$(MAKE)` targets became `cmake --build`, since Ninja
  rejected the unescaped `$`. There's no `--parallel`, which would mean an
  unbounded `-j` with make.
- `PYTHON_EXECUTABLE` is a native path on Windows outside MSYS, because
  `cmd.exe` read `C:/...` after a pipe as a switch.
- `simpgmspace.h`: `sleep()` uses `std::this_thread::sleep_for` under MSVC,
  since there's no `unistd.h`. `radiolib_native` and `simu_drivers` get
  `WIN_INCLUDE_DIRS`.
- `simpgmspace.cpp`: `std::chrono` timer everywhere, as in 2.12. The old
  MSVC `QueryPerformanceCounter` branch was dropped.
- **Port of #6454** (simufatfs to `std::filesystem`). Adaptations: the
  `SIMU_DISKIO` no-op `simuFatfsSetPaths` macro is kept, and the radio
  tests are left untouched. **This also changes the Linux/macOS simulator.**
- **Port of #6478** (`extern "C"` Lua ROTables). MSVC mangles C++ variable
  names. 2.11 has no `colorlib`, so the block starts at `lcdlib`.
- Signature mismatches, which MSVC catches because it mangles return types:
  - Taranis `isBacklightEnabled()` now returns `bool`, as in 2.12.
  - The simu `bluetoothIsWriting()` now returns `uint8_t`.
  - The simu `gyroInit/gyroRead` now return `int` / `-1`, as in 2.12. The
    old `void` stubs left `gyro.cpp`'s `< 0` check reading an undefined
    value.
  - A scan (simu stub definitions vs header declarations) found no others.
- `generate_hwdefs_qrc.py` is run through `PYTHON_EXECUTABLE`, since
  `cmd.exe` can't use its shebang.

Iterate with
`gh workflow run companion.yml --repo pfeerick/edgetx --ref pfeerick/2.11-msvc -f target=windows`
(around 15 minutes with the limiter, about 2 hours for all plugins).

## Next

1. Run the full build (limiter off) with `target=all`. Other boards may hit
   more signature mismatches. Linux and macOS must stay green, because
   #6454, #6478 and the superbuild change touch shared code.
2. Get the radio unit tests to run on the PR. #6454 changes the simulator
   file layer they use.
3. Windows hardware tests (below), and a simulator smoke test on Linux and
   macOS (SD card browsing, Lua scripts, model/settings save).
4. Decide on the order: rdfu PR first, then this one rebased onto 2.11, and
   whether the mingw64 NSIS breakage needs a separate quick fix for 2.11.9.

## Known risks / things to check in CI logs

- `SDL2_LIBRARIES` includes `SDL2::SDL2main`. It linked fine, but check
  at runtime that Companion starts normally.
- pthreads4w and `struct timespec`: no redefinition reported.
- **Bitfield layout:** `-mno-ms-bitfields` is MinGW-only, so packed
  bitfield structs get Microsoft's layout. 2.12 works with that, but watch
  for size `static_assert`s, and test the YAML round-trip.
- About 125 warnings, mostly the MSVC CRT's "deprecated/insecure" ones
  (`sprintf` etc). `-D_CRT_SECURE_NO_WARNINGS` for MSVC builds would quiet
  them.

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
