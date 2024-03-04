cmake_minimum_required(VERSION 3.27)
project(dilemna)

set(CMAKE_CXX_STANDARD 20)

if (NOT DEFINED ENV{WINDIVERT_ROOT_DIR})
    message(FATAL_ERROR "WinDivert not found")
endif()

set(WINDIVERT_INCLUDE_DIR "$ENV{WINDIVERT_ROOT_DIR}/include")
set(WINDIVERT_LIBRARY_PATH "$ENV{WINDIVERT_ROOT_DIR}/x64/WinDivert.lib")

file(GLOB_RECURSE imgui_sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp")

add_executable(dilemna src/main.cpp src/packet/packet_manager.cpp src/ux/menu.cpp src/ux/keyhandler.cpp ${imgui_sources})
target_link_libraries(dilemna PRIVATE ${WINDIVERT_LIBRARY_PATH} ws2_32 d3d11)
target_include_directories(dilemna PRIVATE ${WINDIVERT_INCLUDE_DIR} imgui)