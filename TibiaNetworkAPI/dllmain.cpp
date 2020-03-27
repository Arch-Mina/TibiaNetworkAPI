// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <cstdint>
extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}
#include "WinApi.h"
#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

EXTERN_DLL_EXPORT int main() {
    return 1;
}


lua_State* L;
HANDLE hConsole;
uint32_t baseAddr;
// network addresses
uint32_t getU16Addr = 0x1B79C0;
uint32_t getU32Addr = 0x1B7B60;
uint32_t getU64Addr = 0x1B7D00;
uint32_t addPacketHeadAddr = 0x1B5C10;
uint32_t addU8Addr = 0x1B5FD0;
uint32_t addU16Addr = 0x1B6180;
uint32_t addU32Addr = 0x1B6330;
uint32_t addStringAddr = 0x1B6510;
uint32_t addDoubleAddr = 0x1B6180;
uint32_t sendAddr = 0x1B6B10;

static int addPacketHead(lua_State* L) {
    int n = lua_gettop(L);
    if (n == 0) {
        printf("Missing packethead argument!\n");
        return 0;
    }

    uint32_t packetHead = lua_tonumber(L, 1);

    _asm {
        mov ecx, packetHead;
        call addPacketHeadAddr;
    }

    lua_pushboolean(L, true);
    return 1;
}

static int addString(lua_State* L) {
    int n = lua_gettop(L);
    if (n == 0) {
        printf("Missing addString argument!\n");
        return 0;
    }

    const char* valueString = lua_tostring(L, 1);

    _asm {
        mov ecx, valueString;
        call addStringAddr;
    }

    lua_pushboolean(L, true);
    return 1;
}

static int addU8(lua_State* L) {
    int n = lua_gettop(L);
    if (n == 0) {
        printf("Missing addU8 argument!\n");
        return 0;
    }

    uint32_t valueU8 = lua_tonumber(L, 1);

    _asm {
        mov ecx, valueU8;
        call addU8Addr;
    }

    lua_pushboolean(L, true);
    return 1;
}

static int addU16(lua_State* L) {
    int n = lua_gettop(L);
    if (n == 0) {
        printf("Missing addU16 argument!\n");
        return 0;
    }

    uint32_t valueU16 = lua_tonumber(L, 1);

    _asm {
        mov ecx, valueU16;
        call addU16Addr;
    }

    lua_pushboolean(L, true);
    return 1;
}

static int addU32(lua_State* L) {
    int n = lua_gettop(L);
    if (n == 0) {
        printf("Missing addU16 argument!\n");
        return 0;
    }

    uint32_t valueU32 = lua_tonumber(L, 1);

    _asm {
        mov ecx, valueU32;
        call addU32Addr;
    }

    lua_pushboolean(L, true);
    return 1;
}

static int addDouble(lua_State* L) {
    int n = lua_gettop(L);
    if (n == 0) {
        printf("Missing addDouble argument!\n");
        return 0;
    }

    uint32_t valueDouble = lua_tonumber(L, 1);
    _asm {
        mov ecx, valueDouble;
        call addDoubleAddr;
    }

    lua_pushboolean(L, true);
    return 1;
}

static int send(lua_State* L) {
    int value = 1;
    _asm {
        mov ecx, value;
        call sendAddr;
    }
}

static int debugPrint(lua_State* L) {
    int n = lua_gettop(L);
    if (n == 0) {
        printf("Missing print argument!\n");
        return 0;
    }

    const char* valueString = lua_tostring(L, 1);
    printf("Print LUA: %s\n", valueString);
    lua_pushboolean(L, true);
    return 1;
}

void initializeLua() {
    /* initialize Lua */
    L = lua_open();

    /* load Lua base libraries */
    luaL_openlibs(L);

    /* register our function */
    lua_register(L, "addPacketHead", addPacketHead);
    lua_register(L, "addU8", addU8);
    lua_register(L, "addU16", addU16);
    lua_register(L, "addU32", addU32);
    lua_register(L, "addDouble", addDouble);
    lua_register(L, "addString", addString);
    lua_register(L, "debugPrint", debugPrint);
    lua_register(L, "send", send);
}

void loadScriptThread() {
    luaL_dofile(L, "C:/tnapi/main.lua");
}

void mainLoop() {
    bool insertPressed = false;

    while (1) {
        if (GetAsyncKeyState(VK_INSERT) != 0)
            insertPressed = true;
        else if (insertPressed) {
            insertPressed = false;
            /* run the script */
            if (L != nullptr)
                lua_close(L);

            initializeLua();
            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)loadScriptThread, 0, 0, 0);
            printf("Reloaded script.\n");
        }
    }
}

void initialize() {
    AllocConsole();
    SetConsoleTitle(TEXT("TibiaNetworkAPI"));
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE* dummyStream;
    freopen_s(&dummyStream, "CONOUT$", "wb", stdout);
    freopen_s(&dummyStream, "CONOUT$", "wb", stderr);
    freopen_s(&dummyStream, "CONIN$", "rb", stdin);
    printf("What you gonna do man? i'm watching o_o\n");

    baseAddr = WinApi::GetProcessBaseAddress(GetCurrentProcessId());
    getU16Addr += baseAddr;
    getU32Addr += baseAddr;
    getU64Addr += baseAddr;
    addPacketHeadAddr += baseAddr;
    addU8Addr += baseAddr;
    addU16Addr += baseAddr;
    addU32Addr += baseAddr;
    addDoubleAddr += baseAddr;
    sendAddr += baseAddr;
    addStringAddr += baseAddr;

    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)mainLoop, 0, 0, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        initialize();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

