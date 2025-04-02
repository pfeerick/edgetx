# Fetch yaml-cpp

include(FetchContent)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/gruenich/yaml-cpp
  GIT_TAG        fac0bd8eb5c4972f4284ddebd20c5facb47a32e5
)

FetchContent_MakeAvailable(yaml-cpp)
message("Fetched yaml-cpp source code from Github: ${yaml-cpp_SOURCE_DIR}")
