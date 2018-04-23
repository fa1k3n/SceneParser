find_package(Threads REQUIRED)

set(CMAKE_BUILD_TYPE Debug)

if ($ENV{GOOGLETEST_DIR})
    SET(GOOGLETEST_DIR $ENV{GOOGLETEST_DIR})
else ()
    SET(GOOGLETEST_DIR "${PROJECT_SOURCE_DIR}/external/googletest")
    message(STATUS "using google test at ${GOOGLETEST_DIR}")
endif ()
if (EXISTS ${GOOGLETEST_DIR})
    SET(GTestSrc ${GOOGLETEST_DIR}/googletest)
    SET(GMockSrc ${GOOGLETEST_DIR}/googlemock)
else ()
    message( FATAL_ERROR "No googletest src dir found - set GOOGLETEST_DIR to enable!")
endif ()

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/src")

include_directories(${GTestSrc} ${GTestSrc}/include ${GMockSrc} ${GMockSrc}/include)

cmake_policy(SET CMP0037 OLD)

set(GTEST 
    ${GTestSrc}/src/gtest-all.cc
    ${GMockSrc}/src/gmock-all.cc
	)

set(TESTCASES
    tst_parser.cpp
    tst_tokenizer.cpp
    tst_types.cpp
    )

add_executable(build-tests EXCLUDE_FROM_ALL main.cpp ${TESTCASES} ${GTEST})
target_link_libraries(build-tests scparser Threads::Threads)

add_custom_target(test COMMAND ${CMAKE_BINARY_DIR}/test/build-tests)