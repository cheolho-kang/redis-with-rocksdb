if (NOT __JEMALLOC_INCLUDED) # guard against multiple includes
    set(__JEMALLOC_INCLUDED TRUE)

    find_package(Threads)
    # build directory
    set(jemalloc_PREFIX ${CMAKE_BUILD_DIRECTORY}/deps/jemalloc-prefix)
    # install directory
    set(jemalloc_INSTALL ${CMAKE_BUILD_DIRECTORY}/deps/jemalloc-install)
    set(JEMALLOC_SOURCE_DIR "${PROJECT_SOURCE_DIR}/deps/jemalloc")

    if (UNIX)
        set(JEMALLOC_EXTRA_COMPILER_FLAGS "-fPIC")
    endif()

    set(JEMALLOC_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${JEMALLOC_EXTRA_COMPILER_FLAGS})
    set(JEMALLOC_C_FLAGS ${CMAKE_C_FLAGS} ${JEMALLOC_EXTRA_COMPILER_FLAGS})

    ExternalProject_Add(jemalloc
        SOURCE_DIR ${JEMALLOC_SOURCE_DIR}
        PREFIX ${jemalloc_PREFIX}
        INSTALL_DIR ${jemalloc_INSTALL}
        CONFIGURE_COMMAND ${JEMALLOC_SOURCE_DIR}/configure --enable-autogen --prefix=${jemalloc_INSTALL} --with-xslroot=/opt/homebrew/Cellar/docbook-xsl/1.79.2_1/docbook-xsl
        BUILD_COMMAND make
        INSTALL_COMMAND make dist COMMAND make install
    )
    ExternalProject_Add_Step(jemalloc autoconf
        COMMAND autoconf
        WORKING_DIRECTORY ${JEMALLOC_SOURCE_DIR}
        COMMENT  "Jemalloc autoconf"
        LOG 1
    )
    set(jemalloc_FOUND TRUE)
    set(jemalloc_INCLUDE_DIRS ${jemalloc_INSTALL}/include)
    set(jemalloc_LIBRARIES ${jemalloc_INSTALL}/lib/libjemalloc.a ${CMAKE_THREAD_LIBS_INIT})
endif()
