if (NOT __HIREDIS_INCLUDED) # guard against multiple includes
    set(__HIREDIS_INCLUDED TRUE)

    # build directory
    set(hiredis_PREFIX ${CMAKE_BUILD_DIRECTORY}/deps/hiredis-prefix)
    # install directory
    set(hiredis_INSTALL ${CMAKE_BUILD_DIRECTORY}/deps/hiredis-install)
    set(HIREDIS_SOURCE_DIR "${PROJECT_SOURCE_DIR}/deps/hiredis")

    ExternalProject_Add(hiredis
        SOURCE_DIR ${HIREDIS_SOURCE_DIR}
        PREFIX ${hiredis_PREFIX}
        INSTALL_DIR ${hiredis_INSTALL}
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make static

        BUILD_ALWAYS true
        BUILD_IN_SOURCE true
        INSTALL_COMMAND COMMAND cp libhiredis.a ${hiredis_INSTALL}
    )
    file(GLOB HIREDIS_PUBLIC_HEADERS
        "${HIREDIS_SOURCE_DIR}/*.h"
    )
    file(COPY ${HIREDIS_PUBLIC_HEADERS} DESTINATION ${hiredis_INSTALL}/include)
    set(hiredis_FOUND TRUE)
    set(hiredis_INCLUDE_DIRS ${hiredis_INSTALL}/include)
    set(hiredis_LIBRARIES ${hiredis_INSTALL}/libhiredis.a)
endif()