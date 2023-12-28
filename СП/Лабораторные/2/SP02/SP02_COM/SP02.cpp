#include "SP02.h"
#include "SEQLOG.h"

extern ULONG g_Components;

SP02::SP02() :m_Ref(1)
{
	SEQ;
	InterlockedIncrement((LONG*)&g_Components);
	LOG("SP02::SP02 g_Components = ", g_Components);
}
SP02::~SP02()
{
	SEQ;
	InterlockedDecrement((LONG*)&g_Components);
	LOG("SP02::~SP02 g_Components = ", g_Components);
}

HRESULT STDMETHODCALLTYPE SP02::QueryInterface(REFIID riid, void** ppv)
{
	SEQ;
	HRESULT rc = S_OK;
	*ppv = NULL;
	if (riid == IID_IUnknown) *ppv = (IAdder*)this;
	else if (riid == IID_IAdder) *ppv = (IAdder*)this;
	else if (riid == IID_IMultiplier) *ppv = (IMultiplier*)this;
	else rc = E_NOINTERFACE;
	if (rc == S_OK) this->AddRef();
	LOG("SP02::QueryInterface rc = ", rc);
	return rc;
};
ULONG STDMETHODCALLTYPE SP02::AddRef(void)
{
	SEQ;
	InterlockedIncrement((LONG*)&(this->m_Ref));
	LOG("SP02::AddRef rc = ", this->m_Ref);
	return this->m_Ref;
};
ULONG STDMETHODCALLTYPE SP02::Release(void)
{
	SEQ;
	ULONG rc = this->m_Ref;
	if ((rc = InterlockedDecrement((LONG*)&(this->m_Ref))) == 0) delete this;
	LOG("SP02::Release rc = ", rc);
	return rc;
};

HRESULT STDMETHODCALLTYPE SP02::Add(double x, double y, double& z) { SEQ; LOG("SP02::Add rc = ", S_OK); z = x + y; return S_OK; }
HRESULT STDMETHODCALLTYPE SP02::Sub(double x, double y, double& z) { SEQ; LOG("SP02::Sub rc = ", S_OK); z = x - y; return S_OK; }
HRESULT STDMETHODCALLTYPE SP02::Mul(double x, double y, double& z) { SEQ; LOG("SP02::Mul rc = ", S_OK); z = x * y; return S_OK; }
HRESULT STDMETHODCALLTYPE SP02::Div(double x, double y, double& z) { SEQ; LOG("SP02::Div rc = ", S_OK); z = x / y; return S_OK; }