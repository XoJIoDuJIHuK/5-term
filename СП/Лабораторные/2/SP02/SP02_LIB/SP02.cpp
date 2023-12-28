#include "SP02.h"
#include <functional>
#include "../SP02_COM/INTERFACE.h"

namespace SP02 {
    void ThrowErrorIfFailed(HRESULT hr, IUnknown* pIUnknown) {
        if (FAILED(hr)) {
            pIUnknown->Release();
            throw (int)hr;
        }
    }

    SP02HANDLE Init() {
        IUnknown* pIUnknown = nullptr;
        CoInitialize(nullptr);
        HRESULT hr = CoCreateInstance(
            CLSID_SP02,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_IUnknown,
            (void**)&pIUnknown);
        if (FAILED(hr)) {
            throw (int)hr;
        }
        return pIUnknown;
    }

    namespace Adder {
        double Add(SP02HANDLE h, double x, double y) {
            if (h == nullptr) {
                throw (int)E_INVALIDARG;
            }
            IUnknown* pIUnknown = (IUnknown*)h;
            IAdder* pIAdder = nullptr;
            HRESULT hr = S_OK;
            hr = pIUnknown->QueryInterface(IID_IAdder, (void**)&pIAdder);
            ThrowErrorIfFailed(hr, pIUnknown);

            double z = 0.0;
            hr = pIAdder->Add(x, y, z);
            ThrowErrorIfFailed(hr, pIUnknown);

            pIUnknown->Release();
            return z;
        }

        double Sub(SP02HANDLE h, double x, double y) {
            if (h == nullptr) {
                throw (int)E_INVALIDARG;
            }
            IUnknown* pIUnknown = (IUnknown*)h;
            IAdder* pIAdder = nullptr;
            HRESULT hr = S_OK;
            hr = pIUnknown->QueryInterface(IID_IAdder, (void**)&pIAdder);
            ThrowErrorIfFailed(hr, pIUnknown);

            double z = 0.0;
            hr = pIAdder->Sub(x, y, z);
            ThrowErrorIfFailed(hr, pIUnknown);

            pIUnknown->Release();
            return z;
        }
    }

    namespace Multiplier {
        double Mul(SP02HANDLE h, double x, double y) {
            if (h == nullptr) {
                throw (int)E_INVALIDARG;
            }
            IUnknown* pIUnknown = (IUnknown*)h;
            IMultiplier* pIMultiplier = nullptr;
            HRESULT hr = S_OK;
            hr = pIUnknown->QueryInterface(IID_IMultiplier, (void**)&pIMultiplier);
            ThrowErrorIfFailed(hr, pIUnknown);

            double z = 0.0;
            hr = pIMultiplier->Mul(x, y, z);
            ThrowErrorIfFailed(hr, pIUnknown);

            pIUnknown->Release();
            return z;
        }

        double Div(SP02HANDLE h, double x, double y) {
            if (h == nullptr) {
                throw (int)E_INVALIDARG;
            }
            IUnknown* pIUnknown = (IUnknown*)h;
            IMultiplier* pIMultiplier = nullptr;
            HRESULT hr = S_OK;
            hr = pIUnknown->QueryInterface(IID_IMultiplier, (void**)&pIMultiplier);
            ThrowErrorIfFailed(hr, pIUnknown);

            double z = 0.0;
            hr = pIMultiplier->Div(x, y, z);
            ThrowErrorIfFailed(hr, pIUnknown);

            pIUnknown->Release();
            return z;
        }
    }

    void Dispose(SP02HANDLE h) {
        if (h == nullptr) {
            throw (int)E_INVALIDARG;
        }
        CoFreeUnusedLibraries();
    }
}