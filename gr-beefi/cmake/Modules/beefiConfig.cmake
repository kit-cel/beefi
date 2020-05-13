INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_BEEFI beefi)

FIND_PATH(
    BEEFI_INCLUDE_DIRS
    NAMES beefi/api.h
    HINTS $ENV{BEEFI_DIR}/include
        ${PC_BEEFI_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    BEEFI_LIBRARIES
    NAMES gnuradio-beefi
    HINTS $ENV{BEEFI_DIR}/lib
        ${PC_BEEFI_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/beefiTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BEEFI DEFAULT_MSG BEEFI_LIBRARIES BEEFI_INCLUDE_DIRS)
MARK_AS_ADVANCED(BEEFI_LIBRARIES BEEFI_INCLUDE_DIRS)
