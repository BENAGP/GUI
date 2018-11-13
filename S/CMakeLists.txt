cmake_minimum_required(VERSION 3.2)

PROJECT(S)

#head file path
INCLUDE_DIRECTORIES(
include
)

#source directory
AUX_SOURCE_DIRECTORY(src DIR_SRCS)

#set environment variable
SET(TEST_MATH
${DIR_SRCS}
)

SET(LIBRARIES
libm.so
)


SET(EXECUTABLE_OUTPUT_PATH $(PROJECT_BINARY_DIR}/bin)

SET(FS_BUILD_BINARY_PREFIX "Yfs")


