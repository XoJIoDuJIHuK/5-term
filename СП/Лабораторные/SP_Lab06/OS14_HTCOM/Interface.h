#pragma once
#include "../OS14_HTAPI/pch.h"
#include "../OS14_HTAPI/HT.h"

static const GUID IID_IHT =
// {ED61F249-AA16-49D3-B0C2-F12D8580F6FB}
{ 0xed61f249, 0xaa16, 0x49d3, { 0xb0, 0xc2, 0xf1, 0x2d, 0x85, 0x80, 0xf6, 0xfb } };

//{ 0xe5c68a2d, 0x67f, 0x4240, { 0x9d, 0xbd, 0xfe, 0x4e, 0x4a, 0xa, 0xa6, 0x6c } };

interface IHT : IUnknown {
    STDMETHOD(create(ht::HtHandle** htHandle, int capacity, int secSnapshotInterval, int maxKeyLength, int maxPayloadLength, const wchar_t* htUsersGroup, const wchar_t* fileName)) PURE;
    STDMETHOD(open(ht::HtHandle** htHandle, const wchar_t* fileName, bool isMapFile = false)) PURE;
    STDMETHOD(openAuth(ht::HtHandle** htHandle, const wchar_t* fileName, const wchar_t* htUser, const wchar_t* htPassword, bool isMapFile = false)) PURE;
    STDMETHOD(snap(BOOL& rc, ht::HtHandle* htHandle)) PURE;
    STDMETHOD(close(BOOL& rc, ht::HtHandle* htHandle)) PURE;
    STDMETHOD(insert(BOOL& rc, ht::HtHandle* htHandle, const ht::Element* element)) PURE;
    STDMETHOD(removeOne(BOOL& rc, ht::HtHandle* htHandle, const ht::Element* element)) PURE;
    STDMETHOD(get(ht::Element** rcElement, ht::HtHandle* htHandle, const ht::Element* element)) PURE;
    STDMETHOD(update(BOOL& rc, ht::HtHandle* htHandle, const ht::Element* oldElement, const void* newPayload, int newPayloadLength)) PURE;
    STDMETHOD(getLastError(const char** lastError, ht::HtHandle* htHandle)) PURE;
    STDMETHOD(print(const ht::Element* element)) PURE;
};

static const GUID IID_IElement =
// {AFF858F1-B60E-4249-85E5-172260AB4053}
{ 0xaff858f1, 0xb60e, 0x4249, { 0x85, 0xe5, 0x17, 0x22, 0x60, 0xab, 0x40, 0x53 } };
//{ 0xe5850b11, 0x565c, 0x4812, { 0xa8, 0xc9, 0x3b, 0x62, 0xcc, 0x6e, 0xec, 0x9a } };
 
interface IElement : IUnknown {
    STDMETHOD(createGetElement(ht::Element** getElement, const void* key, int keyLength)) PURE;
    STDMETHOD(createInsertElement(ht::Element** newElement, const void* key, int keyLength, const void* payload, int  payloadLength)) PURE;
    STDMETHOD(createUpdateElement(ht::Element** updateElement, const ht::Element* oldElement, const void* newPayload, int  newPayloadLength)) PURE;
};