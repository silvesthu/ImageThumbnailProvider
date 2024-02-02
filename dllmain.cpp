// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

// Reference
// - https://github.com/Agamnentzar/psd-thumbnail-provider
// - https://github.com/baldurk/renderdoc/blob/v1.x/renderdoc/os/win32/win32_shellext.cpp

#include "ClassFactory.h"

#pragma comment(lib, "Shlwapi.lib")

// {84F5E367-E0A3-4957-B6B5-CA9A2BADC57B}
static CLSID        CLSID_ImageThumbnailProvider = { 0x84f5e367, 0xe0a3, 0x4957, { 0xb6, 0xb5, 0xca, 0x9a, 0x2b, 0xad, 0xc5, 0x7b } };

HINSTANCE           g_hInst = NULL;
long                g_cDllRef = 0;

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Hold the instance of this DLL module, we will use it to get the 
        // path of the DLL to register the component.
        g_hInst = hModule;
        DisableThreadLibraryCalls(hModule);
        OutputDebugStringA("[ImageThumbnailProvider] Attach\n");
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv) {

    OutputDebugStringA("[ImageThumbnailProvider] DllGetClassObject\n");

    HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;

    if (IsEqualCLSID(CLSID_ImageThumbnailProvider, rclsid)) {
        hr = E_OUTOFMEMORY;

        ClassFactory* pClassFactory = new ClassFactory();
        if (pClassFactory) {
            hr = pClassFactory->QueryInterface(riid, ppv);
            pClassFactory->Release();
        }
    }

    return hr;
}

STDAPI DllCanUnloadNow(void) {
    return g_cDllRef > 0 ? S_FALSE : S_OK;
}

STDAPI DllRegisterServer(void) {
    return S_OK;
}

STDAPI DllUnregisterServer(void) {
    return S_OK;
}