#pragma once
#ifdef _WIN64
#pragma comment(lib, "../x64/debug/OS10_HTAPI.lib")
#else
#pragma comment(lib, "../debug/OS10_HTAPI.lib")
#endif

#include "../OS10_HTAPI/pch.h"
#include "../OS10_HTAPI/HT.h"

namespace tests
{
	bool test1(ht::HtHandle* htHandle);
	bool test2(ht::HtHandle* htHandle);
	bool test3(ht::HtHandle* htHandle);
	bool test4(ht::HtHandle* htHandle);
}
