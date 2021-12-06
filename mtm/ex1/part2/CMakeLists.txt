cmake_minimum_required(VERSION 3.0.0)
project(MatamkiyaProject VERSION 0.1.0)

set(CMAKE_C_STANDARD 99)

include(CTest)
enable_testing()

link_libraries(libas.a libmtm.a)

add_executable( ${PROJECT_NAME} amount_set.h list.h matamikya_print.h matamikya_print.c matamikya.h matamikya.c set.h order.h order.c product.c product.h)



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
