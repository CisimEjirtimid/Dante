#include "init.h"

namespace dante::dx
{
    void init(HWND hwnd)
    {
        DevicePtr device;
        auto hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device));
    }
}