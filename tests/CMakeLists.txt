add_executable(static_vector_test static_vector.cpp)
target_link_libraries(static_vector_test plusone doctest)
add_test(static_vector static_vector_test)

add_executable(file_test file.cpp)
target_link_libraries(file_test plusone doctest)
add_test(file file_test)

add_executable(ring_buffer_test ring_buffer.cpp)
target_link_libraries(ring_buffer_test plusone doctest)
add_test(ring_buffer ring_buffer_test)

add_executable(math_test math.cpp)
target_link_libraries(math_test plusone doctest)
add_test(math math_test)

add_executable(socket_test socket.cpp)
target_link_libraries(socket_test plusone doctest)
add_test(socket socket_test)

if (Plusone_USE_SSL)
    add_executable(ssl_test ssl.cpp)
    target_link_libraries(ssl_test plusone doctest)
    add_test(ssl ssl_test)
endif()

add_executable(signal_test signal.cpp)
target_link_libraries(signal_test plusone doctest)
add_test(signal signal_test)

add_executable(process_test process.cpp)
target_link_libraries(process_test plusone doctest)
add_test(process process_test)

add_executable(buffer_test buffer.cpp)
target_link_libraries(buffer_test plusone doctest)
add_test(buffer buffer_test)

add_executable(exception_test exception.cpp)
target_link_libraries(exception_test plusone doctest)
add_test(exception exception_test)

add_executable(static_string_test static_string.cpp)
target_link_libraries(static_string_test plusone doctest)
add_test(static_string static_string_test)
