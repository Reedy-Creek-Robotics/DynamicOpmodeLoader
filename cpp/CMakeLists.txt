cmake_minimum_required(VERSION 3.22.1)
project("ftcrobotcontroller")
add_library(
             ftcrobotcontroller
             SHARED
./src/Functions.cpp
./src/JFunc.cpp
./src/Lua.cpp
./include/lua/lapi.c
./include/lua/lauxlib.c
./include/lua/lbaselib.c
./include/lua/lcode.c
./include/lua/lcorolib.c
./include/lua/lctype.c
./include/lua/ldblib.c
./include/lua/ldebug.c
./include/lua/ldo.c
./include/lua/ldump.c
./include/lua/lfunc.c
./include/lua/lgc.c
./include/lua/linit.c
./include/lua/liolib.c
./include/lua/llex.c
./include/lua/lmathlib.c
./include/lua/lmem.c
./include/lua/loadlib.c
./include/lua/lobject.c
./include/lua/lopcodes.c
./include/lua/loslib.c
./include/lua/lparser.c
./include/lua/lstate.c
./include/lua/lstring.c
./include/lua/lstrlib.c
./include/lua/ltable.c
./include/lua/ltablib.c
./include/lua/ltm.c
./include/lua/lua.c
./include/lua/luac.c
./include/lua/lundump.c
./include/lua/lutf8lib.c
./include/lua/lvm.c
./include/lua/lzio.c
./src/RoadRunner/Marker.cpp
./src/RoadRunner/NodeGrid.cpp
./src/RoadRunner/Save.cpp
        )
target_include_directories(ftcrobotcontroller PRIVATE src include)

find_library(
              log-lib
              log )
target_link_libraries(
                       ftcrobotcontroller

                       ${log-lib} )
