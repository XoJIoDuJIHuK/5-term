#include "INSTALL.h"
#include "SEQLOG.h"
#include "Registry.h"
#include <iostream>
extern HMODULE hmodule;

STDAPI DllInstall(BOOL b, PCWSTR s)
{
	SEQ;
	LOG("DllInstall ", (b)?"install":"uninstall");
	return S_OK;
}
STDAPI DllRegisterServer()
{
	SEQ;
	LOG("DllRegisterServer ", hmodule);
	HRESULT rc = RegisterServer(hmodule, CLSID_SP02, FNAME, VINDX, PRGID);
	LOG("DllRegisterServer rc = ", rc);
	return S_OK;
}
STDAPI DllUnregisterServer()
{
	SEQ;
	LOG("DllUnregister ", hmodule);
	HRESULT rc = UnregisterServer(CLSID_SP02, VINDX, PRGID);
	LOG("UnregisterServer rc = ", rc);
	return S_OK;
}