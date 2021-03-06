#include <windows.h>
#include "detours/detours.h"

#include "Game.h";
#include "Modules/WinAPIModule.h"
#include "Modules/WinsockModule.h"
#include "Config.h"

extern "C" __declspec(dllexport) void NoOp() {}

BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    DisableThreadLibraryCalls(hModule);

    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        Config::Initialize();

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        WinAPIModule::Attach();
        WinsockModule::Attach();

        DetourTransactionCommit();
    }
    
    if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        WinAPIModule::Detach();
        WinsockModule::Detach();
        DetourTransactionCommit();

        Game::OnGameEnd();
    }
    
    return TRUE;
}
