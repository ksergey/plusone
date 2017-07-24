add_executable(clock main.cpp)
target_link_libraries(clock plusone)

add_executable(clock_vdso main_vdso.cpp)
target_link_libraries(clock_vdso plusone dl)
