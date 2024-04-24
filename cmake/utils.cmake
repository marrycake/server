include(${CMAKE_SOURCE_DIR}/cmake/macro.cmake)

set(UTILS_DIR ${CMAKE_SOURCE_DIR}/src/utils)

file(GLOB UTILS_SOURCE "${UTILS_DIR}/*.cpp")
file(GLOB UTILS_HEADER "${UTILS_DIR}/*.h")

set(UTILS_RESOURCE ${UTILS_HEADER} ${UTILS_SOURCE})