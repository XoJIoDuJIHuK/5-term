// COM.cpp
#include "COM.h"
#include "SEQLOG.h"
#include "CFactory.h"

extern ULONG g_Components;
extern ULONG g_ServerLocks;

STDAPI DllGetClassObject(const CLSID& clsid, //clsid <-- CoCreateInstance(CLSID_SP02)
						 const IID& iid, //iid <-- OLE32(ClassFactory iid)
						 void** ppv) //--> ClassFactory ptr
{
	SEQ;
	HRESULT rc = E_UNEXPECTED;
	CFactory* pF;
	if (clsid != CLSID_SP02) rc = CLASS_E_CLASSNOTAVAILABLE;
	else if ((pF = new CFactory()) == NULL) rc = E_OUTOFMEMORY;
	else
	{
		rc = pF->QueryInterface(iid, ppv);
		pF->Release();
	}
	LOG("DllGetClassObject: rc = ", rc);
	return rc;
}

STDAPI DllCanUnloadNow()
{
	SEQ;
	HRESULT rc = E_UNEXPECTED;
	if ((g_ServerLocks == 0) && (g_Components == 0)) rc = S_OK;
	else rc = S_FALSE;
	LOG("DllCanUnloadNow: rc = ", rc);
	return rc;
}