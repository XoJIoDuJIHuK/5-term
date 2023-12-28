#pragma once
#define SP02HANDLE void*

namespace SP02
{
	SP02HANDLE Init();
	namespace Adder
	{
		double Add(SP02HANDLE h, double x, double y);
		double Sub(SP02HANDLE h, double x, double y);
	}
	namespace Multiplier
	{
		double Mul(SP02HANDLE h, double x, double y);
		double Div(SP02HANDLE h, double x, double y);
	}
	void Dispose(SP02HANDLE h);
}