#pragma once
#include <objbase.h>

#define FNAME L"Tch.SP.COM"
#define VINDX L"Tch.SP"
#define PRGID L"Tch.SP.1"

static const GUID CLSID_SP02 = { 0xa8d51566, 0x529d, 0x444d, 
	{ 0xbd, 0x73, 0x5a, 0x48, 0x3b, 0x60, 0x6d, 0x56 }};

static const GUID IID_IAdder = { 0xcd4c61e8, 0xf38, 0x4045, 
	{ 0x9b, 0x65, 0x30, 0xfb, 0x54, 0xe1, 0x9a, 0xad } };

interface IAdder : IUnknown
{
	virtual HRESULT __stdcall Add(double x, double y, double& z) = 0;
	virtual HRESULT __stdcall Sub(double x, double y, double& z) = 0;
};

static const GUID IID_IMultiplier = { 0xebe30627, 0x70b3, 0x40b2, 
	{ 0xae, 0xf, 0x64, 0x25, 0x3b, 0xa4, 0xc2, 0x21 } };
interface IMultiplier : IUnknown
{
	virtual HRESULT __stdcall Mul(double x, double y, double& z) = 0;
	virtual HRESULT __stdcall Div(double x, double y, double& z) = 0;
};