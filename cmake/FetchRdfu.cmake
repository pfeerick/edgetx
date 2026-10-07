# Fetch the rdfu command line tool (https://github.com/EdgeTX/rs-dfu), used by
# Companion on Windows to flash radios in DFU mode in place of dfu-util.
#
# Sets RDFU_EXECUTABLE to the tool, and RDFU_VCRUNTIME to the vcruntime140.dll
# it needs (empty if none was found, rdfu then relies on an installed VC++
# runtime).
#
# Both can be preset to use local copies, e.g. for offline builds.

set(RDFU_VERSION "0.7.2")
set(RDFU_SHA256 "7f30654c5b3407ee777184610cd9b2d8e6a238583db070efa91fd4df197d39c7")
set(RDFU_URL "https://github.com/EdgeTX/rs-dfu/releases/download/v${RDFU_VERSION}/rdfu-Windows-x86_64.exe")

set(RDFU_EXECUTABLE "" CACHE FILEPATH "rdfu.exe to bundle (downloaded if empty)")

if(NOT RDFU_EXECUTABLE)
  set(_rdfu_download "${CMAKE_BINARY_DIR}/rdfu/rdfu.exe")
  # Skipped when the file is already there with the expected hash
  file(DOWNLOAD "${RDFU_URL}" "${_rdfu_download}"
    EXPECTED_HASH SHA256=${RDFU_SHA256}
    TLS_VERIFY ON
    STATUS _rdfu_status)
  list(GET _rdfu_status 0 _rdfu_code)
  if(NOT _rdfu_code EQUAL 0)
    list(GET _rdfu_status 1 _rdfu_error)
    message(FATAL_ERROR "Downloading rdfu ${RDFU_VERSION} failed: ${_rdfu_error}")
  endif()
  # Not cached, so a version bump downloads again in existing build trees
  set(RDFU_EXECUTABLE "${_rdfu_download}")
endif()

message(STATUS "rdfu: ${RDFU_EXECUTABLE}")

# rdfu is built with MSVC and imports VCRUNTIME140.dll, so bundle it from the
# Visual Studio redistributables when they are installed.
set(RDFU_VCRUNTIME "" CACHE FILEPATH "vcruntime140.dll to bundle with rdfu")

if(NOT RDFU_VCRUNTIME)
  set(_pf86 "ProgramFiles(x86)")
  set(_pf86 "$ENV{${_pf86}}")
  if(NOT _pf86)
    set(_pf86 "C:/Program Files (x86)")
  endif()
  find_program(VSWHERE_EXECUTABLE vswhere
    HINTS "${_pf86}/Microsoft Visual Studio/Installer")

  if(VSWHERE_EXECUTABLE)
    execute_process(
      COMMAND "${VSWHERE_EXECUTABLE}" -latest -products * -find
              "VC/Redist/MSVC/*/x64/Microsoft.VC*.CRT/vcruntime140.dll"
      OUTPUT_VARIABLE _vcruntimes
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    string(REPLACE "\r" "" _vcruntimes "${_vcruntimes}")
    string(REPLACE "\n" ";" _vcruntimes "${_vcruntimes}")
    if(_vcruntimes)
      # Several toolset versions can be installed, take the newest
      list(SORT _vcruntimes COMPARE NATURAL ORDER DESCENDING)
      list(GET _vcruntimes 0 _vcruntime)
      string(REPLACE "\\" "/" _vcruntime "${_vcruntime}")
      set(RDFU_VCRUNTIME "${_vcruntime}" CACHE FILEPATH "vcruntime140.dll to bundle with rdfu" FORCE)
    endif()
  endif()
endif()

if(RDFU_VCRUNTIME)
  message(STATUS "rdfu VC++ runtime: ${RDFU_VCRUNTIME}")
else()
  message(WARNING "vcruntime140.dll not found, rdfu will need the VC++ runtime installed")
endif()
