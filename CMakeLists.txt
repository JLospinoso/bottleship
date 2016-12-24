cmake_minimum_required (VERSION 3.1)
project (bottleship)

set (bottleship_SOURCES
        src/server.cpp
        src/dao.cpp
        src/serialization.cpp)
set(bottleship_TEST_FILES
        tests/serialization_test.cpp
        tests/dao_test.cpp)

add_subdirectory(leveldb)
add_subdirectory(googletest)

find_package (Threads)
find_package (Boost COMPONENTS system filesystem REQUIRED)
find_package (websocketpp REQUIRED)
find_library(gmock REQUIRED)
find_library(gtest REQUIRED)
find_library(gtest_main REQUIRED)

add_executable(bottleship src/main.cpp ${bottleship_SOURCES})
add_executable(tst ${bottleship_SOURCES} ${bottleship_TEST_FILES})

target_link_libraries(bottleship ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} leveldb)
target_link_libraries(tst ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} leveldb gmock gtest gtest_main)

include_directories(${WEBSOCKETPP_INCLUDE_DIR})
include_directories(leveldb/include)
include_directories(include)

set_property(TARGET bottleship PROPERTY CXX_STANDARD 14)
set_property(TARGET bottleship PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET tst PROPERTY CXX_STANDARD 14)
set_property(TARGET tst PROPERTY CXX_STANDARD_REQUIRED ON)
