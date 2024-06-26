include(${CMAKE_SOURCE_DIR}/cmake/macro.cmake)

set(DEVICES_DIR ${CMAKE_SOURCE_DIR}/src/devices)

file(GLOB DEVICES_SOURCE "${DEVICES_DIR}/*.cpp")
file(GLOB DEVICES_HEADER "${DEVICES_DIR}/*.h")

set(DEVICES_RESOURCE ${DEVICES_HEADER} ${DEVICES_SOURCE})