cmake_minimum_required (VERSION 3.1)
project (bottleship)

set (bottleship_SOURCES
		main.cpp
		server.cpp
        dao.cpp
		serialization.cpp)
set (bottleship_HEADERS
		server.h
        dao.h
		serialization.h)

find_package (Threads)
find_package (Boost COMPONENTS system filesystem REQUIRED)
find_package (websocketpp REQUIRED)
add_subdirectory(leveldb)

add_executable(bottleship ${bottleship_SOURCES} ${bottleship_HEADERS})

target_link_libraries(bottleship ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} leveldb)
include_directories(${WEBSOCKETPP_INCLUDE_DIR})
include_directories(leveldb/include)

set_property(TARGET bottleship PROPERTY CXX_STANDARD 14)
set_property(TARGET bottleship PROPERTY CXX_STANDARD_REQUIRED ON)
