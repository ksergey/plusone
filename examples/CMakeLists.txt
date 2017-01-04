add_executable(sockets sockets.cpp)
target_link_libraries(sockets plusone)

add_executable(serialization serialization.cpp)
target_link_libraries(serialization plusone)

add_executable(check check.cpp)
target_link_libraries(check plusone)

add_executable(sockets_mmap sockets_mmap.cpp)
target_link_libraries(sockets_mmap plusone)
