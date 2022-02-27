if (NOT __HDR_HISTOGRAM_INCLUDED) # guard against multiple includes
    set(__HDR_HISTOGRAM_INCLUDED TRUE)

    # build directory
    set(hdr_histogram_PREFIX ${CMAKE_BUILD_DIRECTORY}/deps/hdr_histogram-prefix)
    # install directory
    set(hdr_histogram_INSTALL ${CMAKE_BUILD_DIRECTORY}/deps/hdr_histogram-install)
    set(HDR_HISTOGRAM_SOURCE_DIR "${PROJECT_SOURCE_DIR}/deps/hdr_histogram")

    ExternalProject_Add(hdr_histogram
        SOURCE_DIR ${HDR_HISTOGRAM_SOURCE_DIR}
        PREFIX ${hdr_histogram_PREFIX}
        INSTALL_DIR ${hdr_histogram_INSTALL}
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make

        BUILD_ALWAYS true
        BUILD_IN_SOURCE true
        INSTALL_COMMAND ar rcs libhdrhistogram.a hdr_histogram.o COMMAND cp libhdrhistogram.a ${hdr_histogram_INSTALL}
    )
    file(GLOB HDR_HISTOGRAM_PUBLIC_HEADERS
        "${HDR_HISTOGRAM_SOURCE_DIR}/*.h"
    )
    file(COPY ${HDR_HISTOGRAM_PUBLIC_HEADERS} DESTINATION ${hdr_histogram_INSTALL}/include)
    set(hdr_histogram_FOUND TRUE)
    set(hdr_histogram_INCLUDE_DIRS ${hdr_histogram_INSTALL}/include)
    set(hdr_histogram_LIBRARIES ${hdr_histogram_INSTALL}/libhdrhistogram.a)
endif()