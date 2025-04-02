# Fetch maxLibQt source code from Github

include(FetchContent)

FetchContent_Declare(
  maxLibQt
  GIT_REPOSITORY https://github.com/edgetx/maxLibQt
  GIT_TAG        90aa19783d65cdfb0fdfceb6b6b3c5d4e37523a0
)

FetchContent_MakeAvailable(maxLibQt)

message("Fetched maxLibQt source code from Github: ${maxLibQt_SOURCE_DIR}")
include_directories(
  ${maxLibQt_SOURCE_DIR}
  ${maxLibQt_BINARY_DIR}
)
