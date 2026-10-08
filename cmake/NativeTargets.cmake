
option(DISABLE_COMPANION "Disable building companion and simulators" OFF)

# if(WIN32)
#   set(WIN_EXTRA_LIBS_PATH "C:/Programs" CACHE PATH
#     "Base path to extra libs/headers on Windows (SDL & pthreads folders should be in here).")
#   list(APPEND CMAKE_PREFIX_PATH "${WIN_EXTRA_LIBS_PATH}" "${WIN_EXTRA_LIBS_PATH}/SDL")  # hints for FindSDL
# endif()

# libfox
find_package(Fox QUIET)  # QUIET not working on WIN32?
if (FOX_FOUND)
  message(STATUS "Foxlib found at ${FOX_LIBRARY}")
else()
  message("Libfox not found, simu target will not be available")
endif()

if(NOT DISABLE_COMPANION)
  include(QtDefs)
endif(NOT DISABLE_COMPANION)

if(Qt5Core_FOUND OR FOX_FOUND)
  set(SDL2_BUILDING_LIBRARY YES)  # this prevents FindSDL from appending SDLmain lib to the results, which we don't want
  find_package("SDL2")
  if(SDL2_FOUND)
    # find_package("SDL2") does not set a variable holding the path to the location of the SDL2 shared library
    find_file(SDL2_LIB_PATH
              NAMES
                libSDL2.so
                SDL2.dll
                SDL2.dylib
              HINTS
                "/usr/lib/x86_64-linux-gnu"
                ${SDL2_LIBRARY_PATH})
    if(NOT SDL2_LIB_PATH AND TARGET SDL2::SDL2)
      # e.g. the SDL2 VC development package, whose config has no search hints
      get_target_property(_sdl2_dll SDL2::SDL2 IMPORTED_LOCATION_RELEASE)
      if(NOT _sdl2_dll)
        get_target_property(_sdl2_dll SDL2::SDL2 IMPORTED_LOCATION)
      endif()
      if(_sdl2_dll MATCHES "\\.dll$")
        set(SDL2_LIB_PATH "${_sdl2_dll}" CACHE FILEPATH "SDL2 shared library" FORCE)
      endif()
    endif()
    message(STATUS "SDL2 Lib: ${SDL2_LIB_PATH} Libs: ${SDL2_LIBRARIES}; Headers: ${SDL2_INCLUDE_DIRS}")
  else()
    message(STATUS "SDL2 not found! Simulator audio, and joystick inputs, will not work.")
  endif()
endif()

if(Qt5Core_FOUND AND NOT DISABLE_COMPANION)
  if(WIN32)
    # Windows flashes with rdfu, which needs neither dfu-util nor libusb
    include(FetchRdfu)
  else()
    find_package(Libusb1)
    if(LIBUSB1_FOUND)
      find_package(Dfuutil)
    endif()
  endif()

  if(LINUX)
    find_package(Libssl1)
  endif()

  # OpenSSL
  # environment variable set in github workflows and build-edgetx Dockerfile
  if (DEFINED ENV{OPENSSL_ROOT_DIR})
    set(OPENSSL_ROOT_DIR "$ENV{OPENSSL_ROOT_DIR}")
  endif()

  find_package(OpenSSL)
endif()

# Windows-specific includes and libs shared by sub-projects
if(WIN32)
  if(NOT MINGW)
    # The simulator uses pthreads, which MinGW provides but MSVC doesn't, so
    # use pthreads4w (e.g. vcpkg's "pthreads" port, on CMAKE_PREFIX_PATH)
    find_path(PTHREADS4W_INCLUDE_DIR pthread.h)
    find_library(PTHREADS4W_LIBRARY NAMES pthreadVC3 pthreadVC2)
    find_file(PTHREADS4W_DLL NAMES pthreadVC3.dll pthreadVC2.dll PATH_SUFFIXES bin ../bin)
    if(NOT PTHREADS4W_INCLUDE_DIR OR NOT PTHREADS4W_LIBRARY)
      message(FATAL_ERROR "pthreads4w not found, add it to CMAKE_PREFIX_PATH")
    endif()
    message(STATUS "pthreads4w: ${PTHREADS4W_LIBRARY} ${PTHREADS4W_DLL}")
    list(APPEND WIN_INCLUDE_DIRS "${PTHREADS4W_INCLUDE_DIR}")
    list(APPEND WIN_LINK_LIBRARIES "${PTHREADS4W_LIBRARY}")
  endif()
  # TODO: is that still necessary?
  set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES OFF)
  set(CMAKE_C_USE_RESPONSE_FILE_FOR_LIBRARIES OFF)
  set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES OFF)
  set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_LIBRARIES OFF)
endif()

# google tests
include(FetchGtest)

add_custom_target(tests-radio
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/gtests-radio
  DEPENDS gtests-radio
  )

if(Qt5Core_FOUND AND NOT DISABLE_COMPANION)
  add_subdirectory(${COMPANION_SRC_DIRECTORY})
  add_custom_target(tests-companion
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/gtests-companion
    DEPENDS gtests-companion
    )
  add_custom_target(gtests
    DEPENDS gtests-radio gtests-companion
    )
  add_custom_target(tests
    DEPENDS tests-radio tests-companion
  )
else()
  add_custom_target(gtests
    DEPENDS gtests-radio
    )
  add_custom_target(tests
    DEPENDS tests-radio
  )
endif()

set(IGNORE "${ARM_TOOLCHAIN_DIR}")
