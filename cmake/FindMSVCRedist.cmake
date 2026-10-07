# Find the Visual C++ redistributable installer (vc_redist.x64.exe), which the
# Windows installer runs when the installed runtime is missing or older.
#
# Sets MSVC_REDIST_EXE. It can be preset to use a local copy.

set(MSVC_REDIST_EXE "" CACHE FILEPATH "vc_redist.x64.exe to bundle with the installer")

if(NOT MSVC_REDIST_EXE)
  # VCINSTALLDIR is set by vcvarsall.bat, which an MSVC build needs anyway
  set(_vc_dir "$ENV{VCINSTALLDIR}")
  if(_vc_dir)
    string(REPLACE "\\" "/" _vc_dir "${_vc_dir}")
    file(GLOB _vc_redists "${_vc_dir}/Redist/MSVC/*/vc_redist.x64.exe")
    if(_vc_redists)
      # Several toolset versions can be installed, take the newest
      list(SORT _vc_redists COMPARE NATURAL ORDER DESCENDING)
      list(GET _vc_redists 0 _vc_redist)
      set(MSVC_REDIST_EXE "${_vc_redist}" CACHE FILEPATH "vc_redist.x64.exe to bundle with the installer" FORCE)
    endif()
  endif()
endif()

if(MSVC_REDIST_EXE)
  message(STATUS "VC++ redistributable: ${MSVC_REDIST_EXE}")
else()
  message(WARNING "vc_redist.x64.exe not found (is VCINSTALLDIR set?), the installer won't install the VC++ runtime")
endif()
