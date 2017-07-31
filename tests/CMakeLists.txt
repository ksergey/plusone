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
