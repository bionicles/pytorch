// {{header1}}
// {{header2}}

extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

#ifndef _WIN32
    #include <dlfcn.h>
#endif

#include <iostream>
#include <stdexcept>

#include "luaT.h"
#include "THTensor.h"
#include "THStorage.h"
#include "LuaHelper.h"
#include "nnWrapper.h"

using namespace std;

{% set types = {
    'Long': 'long',
    'Float': 'float',
    'Double': 'double',
    'Byte': 'unsigned char'
}
%}

lua_State *luaInit() {
    #ifndef _WIN32
    void *hdl = dlopen("liblua5.1.so", RTLD_NOW | RTLD_GLOBAL);
    if(hdl == 0) {
        cout << dlerror() << endl;
        throw runtime_error(string("Couldnt load liblua5.1.so ") + dlerror());
    } else {
//        cout << "loaded lua library" << endl;
    }
    #endif

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_getglobal(L, "require");
    lua_pushstring(L, "torch");
    lua_call(L, 1, 0);
//    lua_setglobal(L, "torch");

    lua_getglobal(L, "require");
    lua_pushstring(L, "nn");
    lua_call(L, 1, 1);
    lua_setglobal(L, "nn");

    return L;
}
void luaClose(lua_State *L) {
    lua_close(L);
}
void collectGarbage(lua_State *L) {
    pushGlobal(L, "collectgarbage");
    lua_call(L, 0, 0);
}

{% for Real in types %}
int TH{{Real}}Storage_getRefCount(TH{{Real}}Storage *self) {
    return self->refcount;
}
int TH{{Real}}Tensor_getRefCount(TH{{Real}}Tensor *self) {
    return self->refcount;
}
{% endfor %}

long pointerAsInt(void *ptr) {
    return (long)ptr;
}
