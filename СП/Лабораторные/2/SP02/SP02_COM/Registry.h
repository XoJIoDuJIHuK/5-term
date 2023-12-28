#include "INTERFACE.h"

STDAPI DllInstall(BOOL b, PCWSTR s);
STDAPI DllRegisterServer();
STDAPI DllUnregisterServer();

HRESULT RegisterServer(
	HMODULE hModule,
	const CLSID& clsid,
	const WCHAR* szFriendlyName,
	const WCHAR* szVerIndProgID,
	const WCHAR* szProgID);

HRESULT UnregisterServer(
	const CLSID& clsid,
	const WCHAR* szVerIndProgID,
	const WCHAR* szProgID);