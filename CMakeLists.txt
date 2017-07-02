cmake_minimum_required(VERSION 3.3)

project(Cucumber-CPP)

#
# Generic Compiler Flags
#

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

#
# Dependencies
#

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost "1.63" COMPONENTS thread system regex date_time program_options filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(CUKE_DEPENDENCIES ${Boost_LIBRARIES} ${CMAKE_DL_LIBS})

#find_package(Threads REQUIRED)
#set(CUKE_DEPENDENCIES ${CUKE_DEPENDENCIES} Threads::Threads)

#
# Cucumber-Cpp
#

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#add_subdirectory(src)

add_executable(cucumber-cpp
        src/main.cpp
        src/engine.cpp
        src/extensions.cpp
        src/plugins/plugins.cpp
        src/plugins/WirePlugin.cpp
        src/glue/registration.cpp)
target_link_libraries(cucumber-cpp ${CUKE_DEPENDENCIES})

add_library(some-cucumber-plugin SHARED src/plugins/SomePlugin.cpp)
target_link_libraries(some-cucumber-plugin ${Boost_LIBRARIES})

add_library(glue-example1 SHARED glue-examples/example1.cpp)

add_library(glue-example2 SHARED glue-examples/example2.c)
