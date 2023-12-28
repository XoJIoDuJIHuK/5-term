#include <fstream>
#include <objbase.h>

HMODULE hmodule;
LONG Seq = 0;
std::fstream LogCOM;
#define SEQ LONG __XXCSeq=InterlockedIncrement(&Seq)
#define LOG(x,y) LogCOM<<__XXCSeq<<":"<<(x)<<(y)<<"\n"

ULONG g_Components = 0;
ULONG g_ServerLocks = 0;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	SEQ;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: hmodule = hModule; //debug
							 LogCOM.open("d:\\logCOM.txt", std::ios_base::out); //debug
							 break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH: LogCOM.close(); break; //debug
	}
	return TRUE;
}