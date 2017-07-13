file(GLOB_RECURSE sources "*.cpp")
file(GLOB_RECURSE headers "*.hpp")
set(target "net_resolv")

add_executable(${target} ${sources} ${headers})
target_link_libraries(${target} plusone)
