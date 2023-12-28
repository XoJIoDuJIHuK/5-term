#include "CFactory.h"
#include "SEQLOG.h"
#include "SP02.h"

extern ULONG g_ServerLocks;

CFactory::CFactory() :m_Ref(1) { SEQ; LOG("CFactory::CFactory", ""); };
CFactory::~CFactory()		   { SEQ; LOG("CFactory::~CFactory", "");};

HRESULT STDMETHODCALLTYPE CFactory::QueryInterface(REFIID riid, void** ppv)
{
	SEQ;
	HRESULT rc = S_OK; //COM
	*ppv = NULL; //COM
	if (riid == IID_IUnknown)			*ppv = (IUnknown*)this;
	else if (riid == IID_IClassFactory) *ppv = (IClassFactory*)this;
	else rc = E_NOINTERFACE;

	if (rc == S_OK) this->AddRef();
	LOG("CFactory::QueryInterface rc = ", rc);
	return rc;
}
ULONG STDMETHODCALLTYPE CFactory::AddRef(void)
{
	SEQ;
	InterlockedIncrement((LONG*)&(this->m_Ref));
	LOG("CFactory:AddRef rc = ", this->m_Ref);
	return this->m_Ref;
};
ULONG STDMETHODCALLTYPE CFactory::Release(void)
{
	SEQ;
	ULONG rc = this->m_Ref;
	if ((rc = InterlockedDecrement((LONG*)&(this->m_Ref))) == 0) delete this;
	LOG("CFactory::Release rc = ", rc);
	return rc;
}
HRESULT STDMETHODCALLTYPE CFactory::LockServer(BOOL b) // debug
{
	SEQ;
	HRESULT rc = S_OK;
	if (b) InterlockedIncrement((LONG*)&(g_ServerLocks));
	else InterlockedDecrement((LONG*)&(g_ServerLocks));
	LOG("CFactory::LockServer rc = ", rc);
	return rc;
};
HRESULT STDMETHODCALLTYPE CFactory::CreateInstance(IUnknown* pUO, const IID& id, void** ppv)
{
	SEQ;
	HRESULT rc = E_UNEXPECTED;
	SP02* pA;
	if (pUO != NULL) rc = CLASS_E_NOAGGREGATION;
	else if ((pA = new SP02()) == NULL) rc = E_OUTOFMEMORY;
	else
	{
		rc = pA->QueryInterface(id, ppv);
		pA->Release();
	}
	LOG("CFactory::CreateInstance rc = ", rc);
	return rc;
};