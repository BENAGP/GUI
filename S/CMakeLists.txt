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

ADD_EXECUTABLE(${PROJECT_NAME} ${TEST_MATH})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} m)
