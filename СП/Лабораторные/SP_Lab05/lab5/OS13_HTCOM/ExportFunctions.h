#pragma once

extern HMODULE g_hModule;								// Описатель модуля
extern const wchar_t* g_szFriendlyName;					// Дружественное имя компонента
extern const wchar_t* g_szVerIndProgID;					// Не зависящий от версии ProgID
extern const wchar_t* g_szProgID;						// ProgID
extern long g_cComponents;							// Количество активных компонентов
extern long g_cServerLocks;							// Счетчик блокировок


STDAPI DllCanUnloadNow(); //можно ли выгрузить DLL из памяти

STDAPI DllGetClassObject(const CLSID& clsid, const IID& iid, void** ppv);

STDAPI DllRegisterServer();

STDAPI DllUnregisterServer();