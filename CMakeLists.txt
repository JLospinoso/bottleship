cmake_minimum_required (VERSION 3.1)
project (bottleship)

# Download and unpack googletest at configure time
configure_file(cmake/gtest/CMakeLists.txt.in cmake/gtest/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/cmake/gtest )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/cmake/gtest )
 
# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
 
# Add googletest directly to our build
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build)
 
add_subdirectory(leveldb)

find_package (Threads REQUIRED)
find_package (Boost COMPONENTS system filesystem REQUIRED)
find_package (websocketpp REQUIRED)

set (bottleship_SOURCES
        src/server.cpp
        src/dao.cpp
        src/serialization.cpp)
set(bottleship_TEST_FILES
        tests/serialization_test.cpp
        tests/dao_test.cpp)

add_executable(bottleship src/main.cpp ${bottleship_SOURCES})
add_executable(tst ${bottleship_SOURCES} ${bottleship_TEST_FILES})

target_link_libraries(bottleship ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} leveldb)
target_link_libraries(tst ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} leveldb gmock gtest gtest_main)

include_directories(${WEBSOCKETPP_INCLUDE_DIR})
include_directories(leveldb/include)
include_directories(include)

set_property(TARGET bottleship PROPERTY CXX_STANDARD 14)
set_property(TARGET bottleship PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET tst PROPERTY CXX_STANDARD 14)
set_property(TARGET tst PROPERTY CXX_STANDARD_REQUIRED ON)
