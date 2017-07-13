file(GLOB_RECURSE sources "*.cpp")
file(GLOB_RECURSE headers "*.hpp")
set(target "serialization")

add_executable(${target} ${sources} ${headers})
target_link_libraries(${target} plusone)

file(COPY sample.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
