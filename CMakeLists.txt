cmake_minimum_required(VERSION 3.1)

project(FastRouteISPD2008)
set(BIN_NAME "FastRouteISPD2008")

add_subdirectory(third_party/fastroute)
add_subdirectory(third_party/flute3)

set(CMAKE_CXX_STANDARD 11)

add_executable(${BIN_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(${BIN_NAME}
        FastRoute4.1
        flute
        )

configure_file(etc/POST9.dat POST9.dat COPYONLY)
configure_file(etc/POWV9.dat POWV9.dat COPYONLY)
