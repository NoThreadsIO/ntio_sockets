set(SOURCE_FILES
        ntio/core/task.h
        ntio/core/error.cc
        ntio/core/logger.cc
        ntio/sockets/internal/socket_utils.cc
        ntio/sockets/internal/time_utils.cc
        ntio/sockets/async_accept.cc
        ntio/sockets/async_bind.cc
        ntio/sockets/async_connect.cc
        ntio/sockets/async_read.cc
        ntio/sockets/async_write.cc
        ntio/sockets/end_point.cc
        ntio/sockets/tcp_socket.cc
        ntio/sockets/tcp_listener.cc
        )

set(PRIVATE_HEADERS
        ntio/sockets/internal/socket_utils.h
        ntio/sockets/internal/time_utils.h
        )

set(PUBLIC_HEADERS
        ntio/core/error.h
        ntio/core/logger.h
        ntio/sockets/async_accept.h
        ntio/sockets/async_bind.h
        ntio/sockets/async_connect.h
        ntio/sockets/async_read.h
        ntio/sockets/async_write.h
        ntio/sockets/end_point.h

        ntio/sockets/tcp_listener.h
        ntio/sockets/tcp_socket.h
        )

add_library(ntio_sockets STATIC ${SOURCE_FILES} ${PUBLIC_HEADERS})
add_compile_definitions(ntio_sockets ${NTIO_SOCKETS_LOG_LEVEL})

target_include_directories(ntio_sockets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})