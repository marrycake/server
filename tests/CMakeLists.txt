cmake_minimum_required(VERSION 3.15.5)

project(Intelligent-Aquatic-Living-Server-Test)

include(${CMAKE_SOURCE_DIR}/cmake/devices.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/controller.cmake)

set(COMMON_RESOURCE ${CMAKE_SOURCE_DIR}/src/common.h  ${CMAKE_SOURCE_DIR}/src/utils.cpp ${CMAKE_SOURCE_DIR}/src/utils.h)

add_executable(${PROJECT_NAME} ${COMMON_RESOURCE} ${DEVICES_RESOURCE} ${CONTROLLER_RESOURCE} ${CMAKE_SOURCE_DIR}/tests/client.cpp)
add_executable(DBhelperTest ${COMMON_RESOURCE} ${DEVICES_RESOURCE} ${CONTROLLER_RESOURCE} ${CMAKE_SOURCE_DIR}/tests/DBhelperTest.cpp)

target_link_libraries(DBhelperTest
httplib::httplib
SQLiteCpp
)

add_executable(udpForwarder udpForwarder.cpp)