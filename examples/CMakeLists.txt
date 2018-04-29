include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/examples")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/examples/SceneModel.cpp
    ${PROJECT_SOURCE_DIR}/examples/main.cpp
)

set(HEADERS
     ${PROJECT_SOURCE_DIR}/examples/SceneModel.hpp
)

set(MOCS
     SceneModel.hpp
)

set(OTHER ex1.scene)

set(RESOURCES examples.qrc)
qt5_add_resources(GENERATED_SOURCES ${RESOURCES})
qt5_wrap_cpp(GENERATED_SOURCES ${MOCS})

source_group("Generated Sources - Do Not Edit" FILES ${GENERATED_SOURCES})
include_directories("${CMAKE_BINARY_DIR}")

add_executable(examples
    ${SOURCES}
    ${GENERATED_SOURCES}

    # items included so they show up in your IDE
    ${HEADERS}
    ${MOCS}
    ${RESOURCES}
    ${OTHER}
)
target_link_libraries(examples scparser Qt5::Core Qt5::Quick)
