if (NOT __LINENOISE_INCLUDED) # guard against multiple includes
    set(__LINENOISE_INCLUDED TRUE)

    # build directory
    set(linenoise_PREFIX ${CMAKE_BUILD_DIRECTORY}/deps/linenoise-prefix)
    # install directory
    set(linenoise_INSTALL ${CMAKE_BUILD_DIRECTORY}/deps/linenoise-install)
    set(LINENOISE_SOURCE_DIR "${PROJECT_SOURCE_DIR}/deps/linenoise")

    ExternalProject_Add(linenoise
        SOURCE_DIR ${LINENOISE_SOURCE_DIR}
        PREFIX ${linenoise_PREFIX}
        INSTALL_DIR ${linenoise_INSTALL}
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make

        BUILD_ALWAYS true
        BUILD_IN_SOURCE true
        INSTALL_COMMAND ar rcs liblinenoise.a linenoise.o COMMAND cp liblinenoise.a ${linenoise_INSTALL}
    )
    file(GLOB LINENOISE_PUBLIC_HEADERS
        "${LINENOISE_SOURCE_DIR}/*.h"
    )
    file(COPY ${LINENOISE_PUBLIC_HEADERS} DESTINATION ${linenoise_INSTALL}/include)
    set(linenoise_FOUND TRUE)
    set(linenoise_INCLUDE_DIRS ${linenoise_INSTALL}/include)
    set(linenoise_LIBRARIES ${linenoise_INSTALL}/liblinenoise.a)
endif()