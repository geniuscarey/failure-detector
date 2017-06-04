cmake_minimum_required (VERSION 2.8.11)

project (detector)
set (CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories(./include)
aux_source_directory(src DIR_SRCS)
link_libraries(pthread)
message(${DIR_SRCS})
message(${PROJECT_NAME})
add_executable(${PROJECT_NAME} ${DIR_SRCS})
