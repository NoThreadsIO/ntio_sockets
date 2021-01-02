#enable_testing()

Include(FetchContent)

FetchContent_Declare(
        Catch2
        URL https://github.com/catchorg/Catch2/archive/v2.13.4.zip)

FetchContent_MakeAvailable(Catch2)

set(SOURCE_FILES
        ntio/core/buffer_test.cc
        main.cc)

add_executable(ntio_sockets_test ${SOURCE_FILES})

target_link_libraries(ntio_sockets_test PRIVATE Catch2::Catch2 ntio_sockets)

add_compile_definitions(ntio_sockets_test ${NTIO_SOCKETS_LOG_LEVEL})

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
include(CTest)
include(Catch)
catch_discover_tests(ntio_sockets_test)