#include <testDshow.h>
#include <dshow.h>
#include <atlbase.h>
#include <iostream>
#include <windows.h>
#include <string>
#pragma comment(lib,"Strmiids.lib") 
#include <vector>
std::vector<std::string> device;
bool enumerateDevice()
{

    // enumerate all video capture devices
    CComPtr<ICreateDevEnum> pCreateDevEnum;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pCreateDevEnum);
    if (hr != NOERROR)
    {
        return false;
    }

    CComPtr<IEnumMoniker> pEm;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
    if (hr != NOERROR)
    {
        return false;
    }

    pEm->Reset();
    ULONG cFetched;
    IMoniker *pM;
    int index = 0;
    while (hr = pEm->Next(1, &pM, &cFetched), hr == S_OK)
    {
        IPropertyBag *pBag;
        hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
        if (SUCCEEDED(hr))
        {
            std::string str;
            VARIANT var;
            var.vt = VT_BSTR;
            hr = pBag->Read(L"FriendlyName", &var, NULL);
            if (hr == NOERROR)
            {
                int i = 0;
                while (var.bstrVal[i] != 0x00)
                {
                    str += var.bstrVal[i];
                    i++;
                }
                device.push_back(str);
                std::cout << index << " " << str << std::endl;
                SysFreeString(var.bstrVal);

            }
            pBag->Release();
        }
        pM->Release();
        index++;
    }

    pCreateDevEnum = NULL;
    return true;
}

int main() {
    struct Param {
        int fps;
        int width;
        int height;
    };
    CComPtr<IBaseFilter> m_pDeviceFilter;
    CoInitialize(NULL);
    enumerateDevice();
    
    getchar();
    return 0;
}