﻿# CMakeList.txt : CMake project for cpp-orderbook, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/seahs/repos/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


project ("cpp-orderbook")

# Find SFML package
find_package(SFML COMPONENTS system window graphics REQUIRED)

# Add source to this project's executable.
add_executable (cpp-orderbook "main.cpp" "Order.h" "Orderbook.h" "Orderbook.cpp" "OrderMatchingEngine.h" "OrderMatchingEngine.cpp" "Order.cpp" "OrderbookVisualizer.cpp" "OrderType.h" "Side.h")

# Link SFML libraries
target_link_libraries(cpp-orderbook PRIVATE sfml-system sfml-network sfml-graphics sfml-window)

# Include directories for SFML
include_directories(${SFML_INCLUDE_DIRS})



if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cpp-orderbook PROPERTY CXX_STANDARD 20)
endif()



# TODO: Add tests and install targets if needed.
