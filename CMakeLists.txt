cmake_minimum_required(VERSION 3.9)
project(serwer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS  "-Wall -Wextra -O2" )
set(SOURCE_FILES err.cpp err.h main.cpp Menu.cpp Menu.h TelnetServer.cpp TelnetServer.h Action.h)

add_executable(serwer ${SOURCE_FILES})