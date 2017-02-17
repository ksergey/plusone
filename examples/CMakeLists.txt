add_executable(sockets sockets.cpp)
target_link_libraries(sockets plusone)

add_executable(serialization serialization.cpp)
target_link_libraries(serialization plusone)

add_executable(check check.cpp)
target_link_libraries(check plusone)

add_executable(sockets_mmap sockets_mmap.cpp)
target_link_libraries(sockets_mmap plusone)

add_executable(pcap_read pcap_read.cpp)
target_link_libraries(pcap_read plusone)

add_executable(sockets_udp_test_1 sockets_udp_test_1.cpp)
target_link_libraries(sockets_udp_test_1 plusone)

add_executable(sockets_udp_test_2 sockets_udp_test_2.cpp)
target_link_libraries(sockets_udp_test_2 plusone)

add_executable(arg_parser arg_parser.cpp)
target_link_libraries(arg_parser plusone)
