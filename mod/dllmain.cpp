#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include "tools/DirtyLogger.h"
#include "BDSMod.h"
#include "WorldEditMod.h"

/*
 * set utf-8
 */

void initConsole() {
    system("chcp 65001");
    // enable colorful output
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode;
    GetConsoleMode(hOutput, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;  // NOLINT
    SetConsoleMode(hOutput, dwMode);
}

/*
 * create your mod instance
 */
trapdoor::BDSMod* createBDSModInstance() {
    return new WorldEditMod();
}

/*
 * before the dll inject
 */
void mod_init() {
    initConsole();
    trapdoor::initLogger("sample.log");  //初始化日志
    auto* mod = createBDSModInstance();
    trapdoor::initializeMod(mod);
}

void mod_exit() {}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
    switch (ul_reason_for_call) {  // NOLINT
        case DLL_PROCESS_ATTACH:

            mod_init();

            break;
        case DLL_THREAD_ATTACH:  // NOLINT
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:

            mod_exit();

            break;
    }
    return TRUE;
}
