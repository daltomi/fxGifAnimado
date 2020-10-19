cmake_minimum_required( VERSION 2.8 )

project(GifAnimado)

set( APP GifAnimado)

file(GLOB SOURCES "*.c" "*.h" "*.cpp" "*.hpp")

set ( CMAKE_CXX_COMPILER "g++" )

# BUG: no usar -Ofast
set (CMAKE_CXX_FLAGS_RELEASE "-Wno-unused-function -Wno-unused-variable -DNDEBUG -D__GNU_SOURCE")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DUNICODE -std=c++11 -Os -fno-exceptions -fno-rtti" )
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffunction-sections -fdata-sections" )
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wl,--gc-sections -fmerge-all-constants -fno-ident")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfpmath=387 -mfancy-math-387" )
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffast-math -Wl,-z,norelro")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wl,--build-id=none -Wl,-s")

set( CMAKE_CXX_FLAGS_DEBUG "-DDEBUG -D__GNU_SOURCE -DUNICODE -Wall -std=c++11 -O2 -ggdb -pg -lc")



include (FindPkgConfig)
pkg_search_module(FOX REQUIRED fox17)


include_directories(
    ${FOX_INCLUDE_DIRS} 
    )

link_directories(
    ${FOX_LIBRARY_DIRS} 
    )


add_executable( 
    ${APP} 
    ${SOURCES} 
    ${FOX_INCLUDE_DIRS}
    )

target_link_libraries( 
    ${APP}
    ${FOX_STATIC_LIBRARIES} 
    )

