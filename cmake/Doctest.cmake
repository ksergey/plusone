include(ExternalProject)

set(DOCTEST_ROOT ${CMAKE_CURRENT_BINARY_DIR}/doctest)

ExternalProject_Add(DownloadDoctest
    PREFIX ${DOCTEST_ROOT}
    TMP_DIR ${DOCTEST_ROOT}/temp
    STAMP_DIR ${DOCTEST_ROOT}/stamp
    DOWNLOAD_DIR ${DOCTEST_ROOT}/download
    URL "https://github.com/onqtam/doctest/archive/1.2.6.tar.gz"
    SOURCE_DIR ${DOCTEST_ROOT}/sources
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ""
    INSTALL_DIR ""
)

add_library(doctest INTERFACE)
add_dependencies(doctest DownloadDoctest)
target_include_directories(doctest SYSTEM INTERFACE ${DOCTEST_ROOT}/sources/doctest)
