if (NOT __LUA_INCLUDED) # guard against multiple includes
    set(__LUA_INCLUDED TRUE)

    # build directory
    set(lua_PREFIX ${CMAKE_BUILD_DIRECTORY}/deps/lua-prefix)
    # install directory
    set(lua_INSTALL ${CMAKE_BUILD_DIRECTORY}/deps/lua-install)
    set(LUA_SOURCE_DIR "${PROJECT_SOURCE_DIR}/deps/lua/src")

    set(CFLAGS "${CMAKE_C_FLAGS} -DLUA_ANSI -DENABLE_CJSON_GLOBAL -DREDIS_STATIC='' -DLUA_USE_MKSTEMP")

    ExternalProject_Add(lua
        SOURCE_DIR ${LUA_SOURCE_DIR}
        PREFIX ${lua_PREFIX}
        INSTALL_DIR ${lua_INSTALL}
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make CFLAGS+=${CFLAGS} liblua.a

        BUILD_ALWAYS true
        BUILD_IN_SOURCE true
        INSTALL_COMMAND COMMAND cp liblua.a ${lua_INSTALL}
    )
    file(GLOB LUA_PUBLIC_HEADERS
        "${LUA_SOURCE_DIR}/*.h"
    )
    file(COPY ${LUA_PUBLIC_HEADERS} DESTINATION ${lua_INSTALL}/include)
    set(lua_FOUND TRUE)
    set(lua_INCLUDE_DIRS ${lua_INSTALL}/include)
    set(lua_LIBRARIES ${lua_INSTALL}/liblua.a)
endif()