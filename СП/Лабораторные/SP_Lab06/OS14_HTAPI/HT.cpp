#include "pch.h"

#include "HT.h"

void trace(const char* msg, int r = 8)
{
	std::ofstream out;

	out.open(TRACEPATH, r);
	out << msg << std::endl;

	out.close();
}

void traceLastError(int code)
{
	trace(("Last error code|" + std::to_string(code)).c_str());
}

std::string ptrToString(void* ptr)
{
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(sizeof(void*) * 2) << std::hex 
		<< reinterpret_cast<uintptr_t>(ptr);
	return stream.str();
}

std::string toUTF8(std::wstring wstr)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(wstr.c_str());
}

std::string toUTF8(const wchar_t* wstr)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(wstr);
}

std::wstring GetGlobalName(const wchar_t* str)
{
	std::wstring wstr = str;
	int found = wstr.find_last_of(L"/");
	if (found != -1)
	{
		wstr = wstr.substr(found + 1);
	}
	return L"Global\\" + wstr + L"-filemapping";
	/*return wstr + L"-filemapping";*/
}
std::wstring GetMutexName(const wchar_t* wstr)
{
	return L"Global\\" + std::wstring(wstr) + L"-mutex";
}

std::string GetUsernameUTF8()
{
	wchar_t username[UNLEN + 1];
	DWORD usernameLen = UNLEN + 1;
	GetUserNameW(username, &usernameLen);
	return toUTF8(username);
}

bool SetSecurityInfo(HANDLE hm)
{
	const wchar_t* sdd = L"D:"
		L"(D;OICI;GA;;;BG)" //Deny guests
		L"(D;OICI;GA;;;AN)" //Deny anonymous
		L"(A;OICI;GA;;;AU)" //Allow read, write and execute for Users
		L"(A;OICI;GA;;;BA)"; //Allow all for Administrators
	PSECURITY_DESCRIPTOR pSD;
	if (!ConvertStringSecurityDescriptorToSecurityDescriptorW(sdd, SDDL_REVISION_1, &pSD, NULL))
	{
		DWORD error = GetLastError();
		CloseHandle(hm);
		trace("ConvertStringSecurityDescriptorToSecurityDescriptorW|returned false");
		traceLastError(error);
		return false;
	}

	if (!SetKernelObjectSecurity(hm, DACL_SECURITY_INFORMATION, pSD))
	{
		DWORD error = GetLastError();
		LocalFree(pSD);
		CloseHandle(hm);
		trace("SetSecurityInfo|returned false");
		traceLastError(error);
		return false;
	}

	LocalFree(pSD);
	return true;
}

namespace ht
{
	HtHandle::HtHandle()
	{
		this->capacity = 0;
		this->secSnapshotInterval = 0;
		this->maxKeyLength = 0;
		this->maxPayloadLength = 0;
		ZeroMemory(this->fileName, sizeof(this->fileName));
		this->file = NULL;
		this->fileMapping = NULL;
		this->addr = NULL;
		ZeroMemory(this->lastErrorMessage, sizeof(this->lastErrorMessage));
		this->lastSnaptime = 0;

		this->count = 0;
	}

	HtHandle::HtHandle(int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength, const wchar_t* htUsersGroup, const wchar_t* fileName) : HtHandle()
	{
		this->capacity = capacity;
		this->secSnapshotInterval = secSnapshotInterval;
		this->maxKeyLength = maxKeyLength;
		this->maxPayloadLength = maxPayloadLength;
		memcpy(this->fileName, fileName, sizeof(this->fileName));
		memcpy(this->htUsersGroup, htUsersGroup, sizeof(this->htUsersGroup));
	}

	HtHandle* create(
		int	  capacity,					// емкость хранилища
		int   secSnapshotInterval,		// переодичность сохранения в сек.
		int   maxKeyLength,             // максимальный размер ключа
		int   maxPayloadLength,			// максимальный размер данных
		const wchar_t* htUsersGroup,	// имя группы OS-пользователей
		const wchar_t* fileName)		// имя файла 
	{
		if (canCreateHtFor(htUsersGroup))
		{
			HtHandle* htHandle = createHt(capacity, secSnapshotInterval, maxKeyLength, maxPayloadLength, htUsersGroup, fileName);
			runSnapshotTimer(htHandle);

			return htHandle;
		}
		else
		{
			return NULL;
		}
	}

	HtHandle* createHt(
		int	  capacity,					// емкость хранилища
		int   secSnapshotInterval,		// переодичность сохранения в сек.
		int   maxKeyLength,             // максимальный размер ключа
		int   maxPayloadLength,			// максимальный размер данных
		const wchar_t* htUsersGroup,	// имя группы OS-пользователей
		const wchar_t* fileName)		// имя файла 
	{
		HANDLE hf = CreateFile(
			fileName,
			GENERIC_WRITE | GENERIC_READ,
			NULL,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (hf == INVALID_HANDLE_VALUE)
			throw "create or open file failed";

		int sizeMap = sizeof(HtHandle) + getSizeElement(maxKeyLength, maxPayloadLength) * capacity;
		HANDLE hm = CreateFileMapping(
			hf,
			NULL,
			PAGE_READWRITE,
			0, sizeMap,
			fileName);
		if (!hm)
			throw "create File Mapping failed";

		LPVOID lp = MapViewOfFile(
			hm,
			FILE_MAP_ALL_ACCESS,
			0, 0, 0);
		if (!lp)
			return NULL;

		ZeroMemory(lp, sizeMap);

		HtHandle* htHandle = new(lp) HtHandle(capacity, secSnapshotInterval, maxKeyLength, maxPayloadLength, htUsersGroup, fileName);
		htHandle->file = hf;
		htHandle->fileMapping = hm;
		htHandle->addr = lp;
		htHandle->lastSnaptime = time(NULL);
		htHandle->mutex = CreateMutex(
			NULL,
			FALSE,
			GetMutexName(fileName).c_str());
		if (!htHandle->mutex)
		{
			throw "createHt|mutex is null";
		}
		return htHandle;
	}

	bool canCreateHtFor(const wchar_t* htUsersGroup)
	{
		if (doesUsergroupExist(htUsersGroup))
		{
			if (isCurrentUserInUsergroup(htUsersGroup) && isCurrentUserInUsergroup(L"Администраторы"))
				return true;
		}

		return false;
	}

	HtHandle* open
	(
		const wchar_t* fileName,        // имя файла
		const wchar_t* htUser,			// HT-пользователь
		const wchar_t* htPassword,		// пароль
		bool isMapFile)					// true если открыть fileMapping; false если открыть файл; по умолчанию false
	{
		HtHandle* htHandle = openWithoutAuth(fileName, isMapFile);
		if (htHandle)
		{
			if (!canOpenHt(htHandle, htUser, htPassword))
			{
				close(htHandle);
				return NULL;
			}
		}

		return htHandle;
	}

	HtHandle* open
	(
		const wchar_t* fileName,         // имя файла
		bool isMapFile)					// true если открыть fileMapping; false если открыть файл; по умолчанию false
	{
		HtHandle* htHandle = openWithoutAuth(fileName, isMapFile);
		if (htHandle)
		{
			if (!canOpenHt(htHandle))
			{
				close(htHandle);
				trace("open|can't open ht");
				return NULL;
			}
		}

		return htHandle;
	}

	HtHandle* openWithoutAuth
	(
		const wchar_t* fileName,         // имя файла
		bool isMapFile)					// true если открыть fileMapping; false если открыть файл; по умолчанию false
	{
		std::wstring wFileName = fileName;
		HtHandle* htHandle = NULL;
		trace(("Current user name|" + GetUsernameUTF8()).c_str(), isMapFile ? 8 : std::ofstream::out);
		trace(("openWithoutAuth|" + toUTF8(fileName)).c_str());
		trace(("isMapFile|" + std::string(isMapFile ? "true" : "false")).c_str());

		if (isMapFile)
		{
			trace("openWithoutAuth|trying to open from map name");
			htHandle = openHtFromMapName(fileName);
		}
		else
		{
			trace("openWithoutAuth|trying to open from file");
			htHandle = openHtFromFile(fileName);
			if (htHandle)
			{
				runSnapshotTimer(htHandle);
			}
		}
		
		trace(("openWithoutAuth|htHandle=" + ptrToString(htHandle)).c_str());
		return htHandle;
	}
		
	HtHandle* openHtFromFile(
		const wchar_t* fileName)
	{
		HANDLE hf = CreateFileW(
			fileName,
			GENERIC_WRITE | GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (hf == INVALID_HANDLE_VALUE)
		{
			int code = GetLastError();
			trace("openHtFromFile|hf=NULL");
			traceLastError(code);
			return NULL;
		}

		std::wstring globalName = GetGlobalName(fileName);
		trace(("creating file mapping |" + toUTF8(globalName)).c_str());
		HANDLE hm = CreateFileMappingW(
			hf,
			NULL,
			PAGE_READWRITE,
			0, 0,
			globalName.c_str());
		if (!hm)
		{
			int code = GetLastError();
			trace("openHtFromFile|hm=NULL");
			traceLastError(code);
			return NULL;
		}

		//вот он мой спаситель
		if (!SetSecurityInfo(hm))
		{
			trace("SetSecurityInfo|returned false");
			return NULL;
		}


		LPVOID lp = MapViewOfFile(
			hm,
			FILE_MAP_ALL_ACCESS,
			0, 0, 0);
		if (!lp)
		{
			int code = GetLastError();
			trace("openHtFromFile|lp=NULL");
			traceLastError(code);
			return NULL;
		}

		HtHandle* htHandle = (HtHandle*)lp;
		htHandle->file = hf;
		htHandle->fileMapping = hm;
		htHandle->addr = lp;
		htHandle->lastSnaptime = time(NULL);
		trace((std::string("current mutex|") + ptrToString(htHandle->mutex)).c_str());
		htHandle->mutex = CreateMutexW(
			NULL,
			FALSE,
			fileName);
		if (!htHandle->mutex)
		{
			trace("openHtFromFile|htHandle->mutex is null");
			traceLastError(GetLastError());
			return NULL;
		}
		int code = WaitForSingleObject(htHandle->mutex, 0);
		trace(std::to_string(code).c_str());
		ReleaseMutex(htHandle->mutex);
		trace(("openHtFromFile|htHandle=" + ptrToString(htHandle)).c_str());
		return htHandle;
	}

	HtHandle* openHtFromMapName(
		const wchar_t* fileName)
	{
		std::wstring globalName = GetGlobalName(fileName);
		trace(("opening file mapping |" + toUTF8(globalName)).c_str());
		HANDLE hm = OpenFileMappingW(
			FILE_MAP_ALL_ACCESS,
			false,
			globalName.c_str());
		if (!hm)
		{
			int code = GetLastError();
			trace("openHtFromMapName|hm=NULL");
			traceLastError(code);
			return NULL;
		}

		LPVOID lp = MapViewOfFile(
			hm,
			FILE_MAP_ALL_ACCESS,
			0, 0, 0);
		if (!lp)
		{
			trace("openHtFromMapName|lp=NULL");
			traceLastError(GetLastError());
			return NULL;
		}

		HtHandle* htHandle = new HtHandle();
		memcpy(htHandle, lp, sizeof(HtHandle));
		htHandle->file = NULL;
		htHandle->fileMapping = hm;
		htHandle->addr = lp;
		htHandle->snapshotTimer = NULL;
		trace(("openHtFromMapName|htHandle=" + ptrToString(htHandle)).c_str());
		return htHandle;
	}

	bool canOpenHt(HtHandle* htHandle)
	{
		return isCurrentUserInUsergroup(htHandle->htUsersGroup);
	}

	bool canOpenHt(HtHandle* htHandle, const wchar_t* htUser, const wchar_t* htPassword)
	{
		bool c1 = isUserInUsergroup(htUser, htHandle->htUsersGroup);
		bool c2 = verifyUser(htUser, htPassword);
		trace("canOpenHt|c1=" + c1 ? "true" : "false");
		trace("canOpenHt|c2=" + c2 ? "true" : "false");
		return c1 && c2;
	}

	BOOL runSnapshotTimer(HtHandle* htHandle)
	{
		htHandle->snapshotTimer = CreateWaitableTimer(0, FALSE, 0);
		LARGE_INTEGER Li{};
		Li.QuadPart = -(SECOND * htHandle->secSnapshotInterval);
		SetWaitableTimer(htHandle->snapshotTimer, &Li, htHandle->secSnapshotInterval * 1000, snapAsync, htHandle, FALSE);

		return true;
	}

	void CALLBACK snapAsync(LPVOID prm, DWORD, DWORD)
	{
		HtHandle* htHandle = (HtHandle*)prm;
		if (snap(htHandle))
		{
			trace("spanshotAsync success");
		}
	}

	Element* get     //  читать элемент из хранилища
	(
		HtHandle* htHandle,            // управление HT
		const Element* element)              // элемент 
	{
		WaitForSingleObject(htHandle->mutex, INFINITE);
		int index = findIndex(htHandle, element);
		if (index < 0)
		{
			writeLastError(htHandle, "-- not found element (GET)");
			ReleaseMutex(htHandle->mutex);
			return NULL;
		}

		Element* foundElement = new Element();
		readFromMemory(htHandle, foundElement, index);
		ReleaseMutex(htHandle->mutex);

		return foundElement;
	}

	BOOL insert		// добавить элемент в хранилище
	(
		HtHandle* htHandle,            // управление HT
		const Element* element)              // элемент
	{
		if (htHandle->count >= htHandle->capacity)
		{
			writeLastError(htHandle, "-- not found free memory");
			return false;
		}

		WaitForSingleObject(htHandle->mutex, INFINITE);
		int freeIndex = findFreeIndex(htHandle, element);
		if (freeIndex < 0)
		{
			ReleaseMutex(htHandle->mutex);
			return false;
		}

		writeToMemory(htHandle, element, freeIndex);
		incrementCount(htHandle);
		ReleaseMutex(htHandle->mutex);

		return true;
	}

	BOOL update     //  именить элемент в хранилище
	(
		HtHandle* htHandle,            // управление HT
		const Element* oldElement,          // старый элемент (ключ, размер ключа)
		const void* newPayload,          // новые данные  
		int             newPayloadLength)     // размер новых данных
	{
		WaitForSingleObject(htHandle->mutex, INFINITE);
		int index = findIndex(htHandle, oldElement);
		if (index < 0)
		{
			writeLastError(htHandle, "-- not found element (UPDATE)");
			ReleaseMutex(htHandle->mutex);
			return false;
		}

		Element* updateElement = new Element(oldElement, newPayload, newPayloadLength);
		writeToMemory(htHandle, updateElement, index);
		ReleaseMutex(htHandle->mutex);

		return true;
	}

	BOOL removeOne      // удалить элемент в хранилище
	(
		HtHandle* htHandle,            // управление HT (ключ)
		const Element* element)				 // элемент 
	{
		WaitForSingleObject(htHandle->mutex, INFINITE);
		int index = findIndex(htHandle, element);
		if (index < 0)
		{
			writeLastError(htHandle, "-- not found element (DELETE)");
			ReleaseMutex(htHandle->mutex);
			return false;
		}

		clearMemoryByIndex(htHandle, index);
		std::cout << " index " << index << " ";
		decrementCount(htHandle);
		ReleaseMutex(htHandle->mutex);

		return true;
	}

	BOOL snap         // выполнить Snapshot
	(
		HtHandle* htHandle)           // управление HT (File, FileMapping)
	{
		WaitForSingleObject(htHandle->mutex, INFINITE);
		if (!FlushViewOfFile(htHandle->addr, NULL)) {
			writeLastError(htHandle, "-- snapshot error");
			ReleaseMutex(htHandle->mutex);
			return false;
		}
		htHandle->lastSnaptime = time(NULL);
		ReleaseMutex(htHandle->mutex);
		return true;
	}

	void print                               // распечатать элемент 
	(
		const Element* element)              // элемент 
	{
		std::cout << "Element:" << std::endl;
		std::cout << "{" << std::endl;
		std::cout << "\t\"key\": \"" << (char*)element->key << "\"," << std::endl;
		std::cout << "\t\"keyLength\": " << element->keyLength << "," << std::endl;
		std::cout << "\t\"payload\": \"" << (char*)element->payload << "\"," << std::endl;
		std::cout << "\t\"payloadLength\": " << element->payloadLength << std::endl;
		std::cout << "}" << std::endl;
	}

	BOOL close        // snap и закрыть HT  и  очистить htHandle
	(
		const HtHandle* htHandle)           // управление HT (File, FileMapping)
	{
		HANDLE hf = htHandle->file;
		HANDLE hfm = htHandle->fileMapping;
		HANDLE mutex = htHandle->mutex;

		if (htHandle->snapshotTimer)
		{
			CancelWaitableTimer(htHandle->snapshotTimer);
		}
		UnmapViewOfFile(htHandle->addr);
		CloseHandle(hfm);
		if (hf)
		{
			CloseHandle(hf);
		}
		if (mutex)
		{
			CloseHandle(mutex);
		}

		return true;
	}

	int hashFunction(const char* key, int capacity)
	{
		unsigned long i = 0;
		for (int j = 0; key[j]; j++)
		{
			i += key[j];
		}
		return i % capacity;
	}

	int nextHash(int currentHash, const char* key, int capacity)
	{
		return ++currentHash;
	}

	int findFreeIndex(
		const HtHandle* htHandle,           // управление HT
		const Element* element)				// элемент
	{
		int index = hashFunction((char*)element->key, htHandle->capacity);

		Element* foundElement = new Element();
		do
		{
			if (index >= htHandle->capacity ||
				foundElement->key != NULL &&
				memcmp(foundElement->key, element->key, element->keyLength) == NULL)
			{
				index = -1;
				break;
			}

			readFromMemory(htHandle, foundElement, index);
			index = nextHash(index, (char*)element->key, htHandle->capacity);
		} while (
			(foundElement->keyLength != NULL && foundElement->payloadLength != NULL)
			&& foundElement->keyLength != -1);

		delete foundElement;
		return index - 1;
	}

	int findIndex(
		const HtHandle* htHandle,           // управление HT
		const Element* element)				// элемент
	{
		int index = hashFunction((char*)element->key, htHandle->capacity);

		Element* foundElement = new Element();
		while (true)
		{
			if (index >= htHandle->capacity)
			{
				index = -1;
				break;
			}

			readFromMemory(htHandle, foundElement, index);

			if (foundElement->keyLength == 0)
			{
				index = -1;
				break;
			}
			index = nextHash(index, (char*)element->key, htHandle->capacity);
			if (memcmp(foundElement->key, element->key, element->keyLength) == NULL)
			{
				break;
			}
		}

		delete foundElement;
		return index - 1;
	}

	BOOL writeToMemory(const HtHandle* const htHandle, const Element* const element, int index)
	{
		LPVOID lp = htHandle->addr;

		lp = (HtHandle*)lp + 1;
		lp = (byte*)lp + getSizeElement(htHandle->maxKeyLength, htHandle->maxPayloadLength) * index;

		memcpy(lp, element->key, element->keyLength);
		lp = (byte*)lp + htHandle->maxKeyLength;
		memcpy(lp, &element->keyLength, sizeof(int));
		lp = (int*)lp + 1;
		memcpy(lp, element->payload, element->payloadLength);
		lp = (byte*)lp + htHandle->maxPayloadLength;
		memcpy(lp, &element->payloadLength, sizeof(int));

		return true;
	}

	int incrementCount(HtHandle* const htHandle)
	{
		return ++htHandle->count;
	}

	Element* readFromMemory(const HtHandle* const htHandle, Element* const element, int index)
	{
		LPVOID lp = htHandle->addr;

		lp = (HtHandle*)lp + 1;
		lp = (byte*)lp + getSizeElement(htHandle->maxKeyLength, htHandle->maxPayloadLength) * index;

		element->key = lp;
		lp = (byte*)lp + htHandle->maxKeyLength;
		element->keyLength = *(int*)lp;
		lp = (int*)lp + 1;
		element->payload = lp;
		lp = (byte*)lp + htHandle->maxPayloadLength;
		element->payloadLength = *(int*)lp;

		return element;
	}

	BOOL clearMemoryByIndex(const HtHandle* const htHandle, int index)
	{
		LPVOID lp = htHandle->addr;
		int sizeElement = getSizeElement(htHandle->maxKeyLength, htHandle->maxPayloadLength);

		lp = (HtHandle*)lp + 1;
		lp = (byte*)lp + sizeElement * index;

		memset(lp, 0xff, sizeElement);

		return true;
	}

	int decrementCount(HtHandle* const htHandle)
	{
		return --htHandle->count;
	}

	const char* writeLastError(HtHandle* const htHandle, const char* msg)
	{
		memcpy(htHandle->lastErrorMessage, msg, sizeof(htHandle->lastErrorMessage));
		return htHandle->lastErrorMessage;
	}

	const char* getLastError  // получить сообщение о последней ошибке
	(
		const HtHandle* htHandle)				// управление HT
	{
		return htHandle->lastErrorMessage;
	}
}
