# Inspired by
# https://github.com/KDAB/kdabtv/blob/master/Qt-Widgets-and-more/UnitTests/Tests/CMakeLists.txt

add_library(test-helpers STATIC
    )

target_include_directories(test-helpers
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

target_link_libraries(test-helpers
    PUBLIC
    Qt5::Core
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
            resman-lib
            )
    endforeach()
endfunction()

SETUP_TESTS(
    test_legacy
)
