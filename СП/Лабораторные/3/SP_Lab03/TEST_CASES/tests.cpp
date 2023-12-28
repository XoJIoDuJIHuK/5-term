#include "tests.h"

namespace tests
{
	bool test1(ht::HtHandle* htHandle)
	{
		const char* expected_error = "-- not found element (UPDATE)";
		ht::Element* insertEl = new ht::Element("test1", 6, "test1", 6);
		
		const char* new_payload = "new payload";
		ht::update(htHandle, insertEl, new_payload, strlen(new_payload));
		return strcmp(htHandle->lastErrorMessage, expected_error);

		/*if (
			getEl == NULL ||
			insertEl->keyLength != getEl->keyLength ||
			memcmp(insertEl->key, getEl->key, insertEl->keyLength) != NULL ||
			insertEl->payloadLength != getEl->payloadLength ||
			memcmp(insertEl->payload, getEl->payload, insertEl->payloadLength) != NULL
			)
		{
			
		}*/
	}

	bool test2(ht::HtHandle* htHandle)
	{
		const char* expected_error = "-- not found element (GET)";
		ht::Element* element = new ht::Element("test2", 6, "test2", 6);

		ht::insert(htHandle, element);
		ht::removeOne(htHandle, element);
		ht::get(htHandle, element);
		return strcmp(htHandle->lastErrorMessage, expected_error);
	}

	bool test3(ht::HtHandle* htHandle)
	{
		const char* expected_error = "cannot insert element with existing key";
		ht::Element* element = new ht::Element("test3", 6, "test3", 6);

		ht::insert(htHandle, element);
		ht::insert(htHandle, element);
		return strcmp(htHandle->lastErrorMessage, expected_error);	
	}

	bool test4(ht::HtHandle* htHandle)
	{
		const char* expected_error = "-- not found element (DELETE)";
		ht::Element* element = new ht::Element("test3", 6, "test3", 6);

		ht::insert(htHandle, element);
		ht::removeOne(htHandle, element);
		ht::removeOne(htHandle, element);
		return strcmp(htHandle->lastErrorMessage, expected_error);
	}
}