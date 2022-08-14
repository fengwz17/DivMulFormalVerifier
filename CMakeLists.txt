cmake_minimum_required(VERSION 3.21)
project(btor2Translator VERSION 0.1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

include(FetchContent)
fetchcontent_declare(btor2tools
    GIT_REPOSITORY https://github.com/Boolector/btor2tools.git
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/deps/btor2tools
)
fetchcontent_makeavailable(btor2tools)

include_directories(deps/btor2tools/src)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(btorTrans ${SOURCES})
target_link_libraries(btorTrans btor2parser)
