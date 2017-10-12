file(GLOB_RECURSE sources "*.cpp")
file(GLOB_RECURSE headers "*.hpp")
set(target "ws")

add_executable(${target} ${sources} ${headers})
target_link_libraries(${target} plusone)
