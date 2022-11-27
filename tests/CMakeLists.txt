# Inspired by
# https://github.com/KDAB/kdabtv/blob/master/Qt-Widgets-and-more/UnitTests/Tests/CMakeLists.txt

add_library(resman-test-helpers STATIC
    clientmultitontest.cpp
    clientmultitontest.h
    )

target_include_directories(resman-test-helpers
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

target_link_libraries(resman-test-helpers
    PUBLIC
    Qt5::Core
    resman-lib
    )

enable_testing()

function(SETUP_TESTS)
    foreach(_testname ${ARGN})
        add_test(NAME ${_testname}
                 COMMAND ${_testname})
        add_executable(${_testname}
            ${_testname}.h
            ${_testname}.cpp
            )
        target_include_directories(${_testname}
            PUBLIC
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/resource>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/scpi>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/server>
            )
        target_link_libraries(${_testname}
            PUBLIC
            Qt5::Test
            resman-test-helpers
            )
    endforeach()
endfunction()

SETUP_TESTS(
    test_scpi
)
