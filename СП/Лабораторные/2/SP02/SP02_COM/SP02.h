#pragma once
#include <objbase.h>
#include <iostream>
#include "INTERFACE.h"

class SP02 : public IAdder, public IMultiplier
{
public:
	SP02();
	~SP02();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);

	virtual HRESULT STDMETHODCALLTYPE Add(double x, double y, double& z);
	virtual HRESULT STDMETHODCALLTYPE Sub(double x, double y, double& z);
	virtual HRESULT STDMETHODCALLTYPE Mul(double x, double y, double& z);
	virtual HRESULT STDMETHODCALLTYPE Div(double x, double y, double& z);
private:
	volatile ULONG m_Ref;
};