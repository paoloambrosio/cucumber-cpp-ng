cmake_minimum_required(VERSION 3.1)

project(Cucumber-CPP)

#
# Generic Compiler Flags
#

set(CMAKE_CXX_STANDARD 11)

#
# Dependencies
#

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost "1.63" COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
set(CUKE_DEPENDENCIES ${CUKE_DEPENDENCIES} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_DATE_TIME_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

#find_package(Threads REQUIRED)
#set(CUKE_DEPENDENCIES ${CUKE_DEPENDENCIES} Threads::Threads)

#
# Cucumber-Cpp
#

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#add_subdirectory(src)

add_executable(cucumber-cpp src/main.cpp src/Plugin.cpp src/SomePlugin.cpp src/WirePlugin.cpp)
target_link_libraries(cucumber-cpp ${CUKE_DEPENDENCIES})
