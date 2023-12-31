#pragma once

namespace ht
{
	extern "C" __declspec(dllexport) struct Element   // элемент 
	{
		__declspec(dllexport) Element();
		__declspec(dllexport) Element(const void* key, int keyLength);                                             // for get
		__declspec(dllexport) Element(const void* key, int keyLength, const void* payload, int  payloadLength);    // for insert
		__declspec(dllexport) Element(const Element* oldElement, const void* newPayload, int  newPayloadLength);   // for update
		const void* key;                 // значение ключа 
		int         keyLength;           // рахмер ключа
		const void* payload;             // данные 
		int         payloadLength;       // размер данных
	};

	extern "C" __declspec(dllexport) Element* createGetElement(const void* key, int keyLength);
	extern "C" __declspec(dllexport) Element* createInsertElement(const void* key, int keyLength, const void* payload, int  payloadLength);

	int getSizeElement(
		int   maxKeyLength,                // максимальный размер ключа
		int   maxPayloadLength);           // максимальный размер данных
}