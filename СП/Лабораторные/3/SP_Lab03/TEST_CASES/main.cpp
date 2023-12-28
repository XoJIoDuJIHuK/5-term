#include "tests.h"
#include <iostream>

using namespace std;

int main()
{
	ht::HtHandle* ht = new ht::HtHandle();
	try 
	{
		ht = ht::create(1000, 1, 20, 256, L"./files/HTspace.ht");
		cout << "-- create: success" << endl;
	}
	catch(const char* msg)
	{
		cout << "-- create: error" << endl;
	}

	if (tests::test1(ht))
	{
		cout << "-- test 1 error: " << ht::getLastError(ht) << endl;
		ht::wipeLastError(ht);
	}
	else
	{
		cout << "-- test 1 success" << endl;
	}

	
	if (tests::test2(ht))
	{
		cout << "-- test 2 error: " << ht::getLastError(ht) << endl;
		ht::wipeLastError(ht);
	}
	else
	{
		cout << "-- test 2 success" << endl;
	}

	if (tests::test3(ht))
	{
		cout << "-- test 3 error: " << ht::getLastError(ht) << endl;
		ht::wipeLastError(ht);
	}
	else
	{
		cout << "-- test 3 success" << endl;
	}

	if (tests::test4(ht))
	{
		cout << "-- test 4 error: " << ht::getLastError(ht) << endl;
		ht::wipeLastError(ht);
	}
	else
	{
		cout << "-- test 4 success" << endl;
	}

	if (ht != nullptr)
		if (ht::close(ht))
			cout << "-- close: success" << endl;
		else
			throw "-- close: error";
}