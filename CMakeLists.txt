cmake_minimum_required(VERSION 3.20)
project(rhytm_test)
add_executable(${PROJECT_NAME} src/${PROJECT_NAME}.cpp src/lexer.cpp src/parser.cpp src/codegen.cpp src/scheme.cpp src/utils.cpp src/topological_sort.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include enum)
