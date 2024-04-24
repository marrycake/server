cmake_minimum_required(VERSION 3.15.5)

project(Intelligent-Aquatic-Living-Server)

set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_SOURCE_DIR}/cmake/devices.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/controller.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/utils.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/code-coverage.cmake)

print_resource("DEVICES_HEADER" "${DEVICES_HEADER}" "-----------------------------------")
print_resource("DEVICES_SOURCE" "${DEVICES_SOURCE}" "-----------------------------------")

print_resource("CONTROLLER_HEADER" "${CONTROLLER_HEADER}" "-----------------------------------")
print_resource("CONTROLLER_SOURCE" "${CONTROLLER_SOURCE}" "-----------------------------------")

print_resource("UTILS_HEADER" "${UTILS_HEADER}" "-----------------------------------")
print_resource("UTILS_SOURCE" "${UTILS_SOURCE}" "-----------------------------------")

add_subdirectory(${CMAKE_SOURCE_DIR}/third_partys/cpp-httplib)
add_subdirectory(${CMAKE_SOURCE_DIR}/third_partys/SQLiteCpp)


include(FetchContent)

FetchContent_Declare(GSL
    GIT_REPOSITORY "https://github.com/microsoft/GSL"
    GIT_TAG "v4.0.0"
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(GSL)

set(COMMON_RESOURCE ${CMAKE_SOURCE_DIR}/src/common.h ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_executable(${PROJECT_NAME} ${COMMON_RESOURCE} ${DEVICES_RESOURCE} ${CONTROLLER_RESOURCE} ${UTILS_RESOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC ${UTILS_DIR} ${CONTROLLER_DIR} ${DEVICES_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE
httplib::httplib
  SQLiteCpp
  sqlite3
  pthread
  dl
  Microsoft.GSL::GSL
  )

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/third_partys/json/include)
target_code_coverage(${PROJECT_NAME})


