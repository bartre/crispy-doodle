cmake_minimum_required(VERSION 3.16 FATAL_ERROR)


set(FETCHCONTENT_QUIET FALSE)
include(FetchContent)
FetchContent_Declare(
googletest
GIT_REPOSITORY https://github.com/google/googletest.git
GIT_TAG release-1.11.0
GIT_PROGRESS TRUE
)
FetchContent_GetProperties(googletest)
option(ENABLE_UNIT_TESTS "Enable unit tests" ON)
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")
if(ENABLE_UNIT_TESTS)
    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        # Prevent GoogleTest from overriding our compiler/linker options
        # when building with Visual Studio
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        # Prevent GoogleTest from using PThreads
        set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)
        # adds the targers: gtest, gtest_main, gmock, gmock_main
        add_subdirectory(
        ${googletest_SOURCE_DIR}
        ${googletest_BINARY_DIR}
        )
        # Silence std::tr1 warning on MSVC
        if(MSVC)
            foreach(_tgt gtest gtest_main gmock gmock_main)
                target_compile_definitions(${_tgt}
                PRIVATE
                "_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING"
                )
            endforeach()
        endif()
    endif()
endif()

add_library(chess STATIC chess_engine.c)
add_executable(chess_engine main.c)
target_link_libraries(chess_engine chess)

add_executable(chess_engine_unit_tests "")

target_sources(chess_engine_unit_tests
    PRIVATE
    ce_ut.cpp
)
target_link_libraries(chess_engine_unit_tests
    PRIVATE
    chess
    gtest_main
)

enable_testing()
add_test(
    NAME chess_engine_unit_tests
    COMMAND $<TARGET_FILE:chess_engine_unit_tests>
)