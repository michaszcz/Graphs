cmake_minimum_required(VERSION 3.9)
project(Graphs)
set(EXECUTABLE_NAME "Graphs")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -DWX_PRECOMP")

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(wxWidgets COMPONENTS core base REQUIRED)

include(${wxWidgets_USE_FILE})
include_directories(include/ ${wxWidgets_INCLUDE_DIRS})

file(GLOB SOURCES "src/*.cpp")

add_executable(${EXECUTABLE_NAME} ${SOURCES})
target_link_libraries(${EXECUTABLE_NAME}  ${wxWidgets_LIBRARIES})