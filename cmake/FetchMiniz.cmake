# Fetch miniz source code from Github

include(FetchContent)

FetchContent_Declare(
  miniz
  GIT_REPOSITORY https://github.com/pfeerick/miniz/
  GIT_TAG        8061c01970016f60eba4e4c563aab09290523878
)

FetchContent_MakeAvailable(miniz)

message("Fetched miniz source code from Github: ${miniz_SOURCE_DIR}")
include_directories(
  ${miniz_SOURCE_DIR}
  ${miniz_BINARY_DIR}
)
