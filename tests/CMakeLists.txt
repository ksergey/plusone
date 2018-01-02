include(Doctest)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -march=native -mtune=native")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -DNDEBUG")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
endif()

add_executable(static_vector_test static_vector.cpp)
target_link_libraries(static_vector_test plusone doctest ubsan)
target_compile_options(static_vector_test PRIVATE -fsanitize=undefined)
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

add_executable(signal_test signal.cpp)
target_link_libraries(signal_test plusone doctest)
add_test(signal signal_test)

add_executable(buffer_test buffer.cpp)
target_link_libraries(buffer_test plusone doctest)
add_test(buffer buffer_test)

add_executable(static_string_test static_string.cpp)
target_link_libraries(static_string_test plusone doctest)
add_test(static_string static_string_test)

add_executable(tuple_test tuple.cpp)
target_link_libraries(tuple_test plusone doctest)
add_test(tuple tuple_test)

add_executable(fixed_size_memory_pool_test fixed_size_memory_pool.cpp)
target_link_libraries(fixed_size_memory_pool_test plusone doctest)
add_test(fixed_size_memory_pool fixed_size_memory_pool_test)

add_executable(build_test build.cpp)
target_link_libraries(build_test plusone doctest)
add_test(build build_test)

add_executable(lambda_test lambda.cpp)
target_link_libraries(lambda_test plusone)
add_test(lambda lambda_test)

add_executable(linear_allocator_test linear_allocator.cpp)
target_link_libraries(linear_allocator_test plusone doctest)
add_test(linear_allocator linear_allocator_test)
