cmake_minimum_required(VERSION 3.0.0)
project(AmountSet VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(AmountSet Test.c amount_set_str.c amount_set_str.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
