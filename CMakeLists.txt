cmake_minimum_required(VERSION 3.21)
project(btor2Translator VERSION 0.1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(deps/btor2tools/src/btor2parser/)
link_directories(deps/btor2tools/build/lib/)
add_executable(btorTrans btor2Translator.cpp)

target_link_libraries(btorTrans libbtor2parser.so)