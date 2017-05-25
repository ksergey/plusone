add_subdirectory(mmap_rx)
add_subdirectory(dispatch)

if (PLUSONE_USE_CURL)
    add_subdirectory(curl)
endif()
