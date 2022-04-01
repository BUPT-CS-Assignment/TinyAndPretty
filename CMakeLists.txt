cmake_minimum_required(VERSION 3.1.6)
project(taptap CXX)

set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

file(GLOB_RECURSE SRC_DIR ${PROJECT_SOURCE_DIR}/src/*.cpp)
#file(GLOB_RECURSE INC_DIR ${PROJECT_SOURCE_DIR/include/*.h} ${PROJECT_SOURCE_DIR/include/*.hpp})

include_directories(${PROJECT_SOURCE_DIR}/include)

link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(${PROJECT_NAME} ${SRC_DIR})

target_link_libraries(${PROJECT_NAME} ne)

