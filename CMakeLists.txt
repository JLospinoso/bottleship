cmake_minimum_required (VERSION 3.1)
project (bottleship)

set (bottleship_SOURCES
	main.cpp
	server.cpp
)
set (bottleship_HEADERS
	server.h
)

find_package (Boost COMPONENTS system filesystem REQUIRED)
find_package (websocketpp REQUIRED)

add_executable(bottleship ${bottleship_SOURCES} ${bottleship_HEADERS})

target_link_libraries(bottleship ${Boost_SYSTEM_LIBRARY})
include_directories(${WEBSOCKETPP_INCLUDE_DIR})

set_property(TARGET bottleship PROPERTY CXX_STANDARD 14)
set_property(TARGET bottleship PROPERTY CXX_STANDARD_REQUIRED ON)
