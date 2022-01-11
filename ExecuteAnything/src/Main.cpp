// Main.cpp : Defines the entry point for the DLL application.
//

#include "core/base.h"
#include "ExecuteAnything.h"


extern "C" BOOL APIENTRY DllMain( 
                        HINSTANCE hInstance, 
                        DWORD     dwReason, 
                        LPVOID    lpReserved
					 )
{
    switch ( dwReason )
    {
        case DLL_PROCESS_ATTACH:
            ExecuteAnything::Get_Program().OnDllProcessAttach(hInstance);
            break;

        case DLL_PROCESS_DETACH:
            ExecuteAnything::Get_Program().OnDllProcessDetach();
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;

        default:
            break;
    }
    
    return TRUE;
}

extern "C" __declspec(dllexport) void setInfo(NppData nppd)
{
    ExecuteAnything::Get_Program().nppSetInfo(nppd, ExecuteAnything::PLUGIN_NAME );
}

extern "C" __declspec(dllexport) const TCHAR * getName()
{
    return ExecuteAnything::PLUGIN_NAME;
}

extern "C" __declspec(dllexport) void beNotified(SCNotification* pscn)
{
    ExecuteAnything::Get_Program().nppBeNotified(pscn);
}

extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam)
{
    return ExecuteAnything::Get_Program().nppMessageProc(Message, wParam, lParam);
}

extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int* pnbFuncItems)
{
    return ExecuteAnything::Get_Program().nppGetFuncsArray(pnbFuncItems);
}

#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode()
{
    return TRUE;
}
#endif
