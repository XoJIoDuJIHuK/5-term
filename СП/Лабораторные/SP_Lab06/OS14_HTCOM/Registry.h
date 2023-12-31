#pragma once
static const GUID CLSID_ComponentHT =
// {FD1025A0-F28A-4F81-8102-0D8A32127E9C}
{ 0xfd1025a0, 0xf28a, 0x4f81, { 0x81, 0x2, 0xd, 0x8a, 0x32, 0x12, 0x7e, 0x9c } };
//{ 0x693c66d5, 0x3ce5, 0x4512, { 0xa3, 0x11, 0x78, 0x26, 0x18, 0xdb, 0xc7, 0xa6 } };

HRESULT RegisterServer(HMODULE hModule,            // DLL module handle
	const CLSID& clsid,         // Class ID
	const WCHAR* szFriendlyName, // Friendly Name
	const WCHAR* szVerIndProgID, // Programmatic
	const WCHAR* szProgID);       //   IDs

HRESULT UnregisterServer(const CLSID& clsid,
	const WCHAR* szVerIndProgID,
	const WCHAR* szProgID);