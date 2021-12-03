cmake_minimum_required(VERSION 3.0.0)
project(HW1 VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(HW1 bp.cpp bp_main.c bp_api.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
