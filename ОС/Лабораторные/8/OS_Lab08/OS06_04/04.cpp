#include <iostream>
#include <Windows.h>
using namespace std;



void sh(HANDLE pheap)
{
	PROCESS_HEAP_ENTRY phe;
	phe.lpData = NULL;
	while (HeapWalk(pheap, &phe))
	{
		// R - начало непрерывной области
		// U - нераспределённая область 
		// B - распределённая область
		cout << "address = " << showbase << hex << phe.lpData <<
			", size = " << dec << phe.cbData <<
			((phe.wFlags & PROCESS_HEAP_REGION) ? " R" : "") <<
			((phe.wFlags & PROCESS_HEAP_UNCOMMITTED_RANGE) ? " U" : "") <<
			((phe.wFlags & PROCESS_HEAP_ENTRY_BUSY) ? " B" : "") << "\n";
	}
}



int main()
{
	HANDLE pheap = GetProcessHeap();
	cout << "before" << endl << endl << endl;
	sh(pheap);

	int size = 300000;
	int* m = new int[size];

	cout << endl << endl << endl << "after" << endl << endl << endl;
	sh(pheap);
	delete[] m;
}