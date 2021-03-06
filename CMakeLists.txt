cmake_minimum_required(VERSION 3.5)

project(SceneParser LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if (COVERAGE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif()

if(NOT DEFINED EIGEN3_INCLUDE_DIRS)
    find_package (Eigen3 3.3  REQUIRED NO_MODULE)
endif()
include_directories(SYSTEM ${EIGEN3_INCLUDE_DIRS})

add_subdirectory("test")
add_subdirectory("examples")

set(SRC
  src/SceneParser.cpp
  src/Tokenizer.cpp
)

set(PUBLIC_INC
    include/ISceneGenerator.hpp
    include/Types.hpp
    include/SceneParser.hpp
)

set(PRIVATE_INC
    src/Tokenizer.hpp
    src/PrivTypes.hpp
)

include_directories(include src)

add_library(scparser ${SRC} ${PUBLIC_INC} ${PRIVATE_INC})
