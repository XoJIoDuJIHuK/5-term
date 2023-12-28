#pragma once
#include <objbase.h>

class CFactory : public IClassFactory
{
public:
	//IUnknown
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);//get interface
	virtual ULONG STDMETHODCALLTYPE AddRef(void);//increment links count
	virtual ULONG STDMETHODCALLTYPE Release(void);//decrement links count
	//IClassFactory
	virtual HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown* pUO, const IID& id, void **ppv);//create component
	virtual HRESULT STDMETHODCALLTYPE LockServer(BOOL b);//block server
	CFactory();
	~CFactory();
private:
	ULONG m_Ref;
};