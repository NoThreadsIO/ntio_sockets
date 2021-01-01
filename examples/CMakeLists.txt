add_executable(
        simple_example
        simple_example.cc
)

target_link_libraries(
        simple_example
        ntio_sockets
)

add_compile_definitions(simple_example LOG_LEVEL_DEBUG)