#pragma once

#include <Windows.h>
#include <iostream>

namespace ht
{
	struct Element   
	{
		Element();
		Element(const void* key, int keyLength);                                             // for get
		Element(const void* key, int keyLength, const void* payload, int  payloadLength);    // for insert
		Element(const Element* oldElement, const void* newPayload, int  newPayloadLength);   // for update
		const void* key;                 
		int         keyLength;           
		const void* payload;             
		int         payloadLength;       
	};

	int getSizeElement(
		int   maxKeyLength,               
		int   maxPayloadLength);          
}