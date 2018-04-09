cmake_minimum_required(VERSION 2.8.12)

project(SceneParser LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_subdirectory("test")

set(SRC
  src/SceneParser.cpp
  src/Tokenizer.cpp
)

set(INC
    include/SceneParser.hpp
    include/Tokenizer.hpp
    include/ISceneGenerator.hpp
    include/Types.hpp
)

include_directories("include")

add_library(scparser ${SRC} ${INC})
