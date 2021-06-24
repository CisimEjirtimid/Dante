#pragma once
#include "common.h"
#include <windef.h>

namespace dante::dx
{
    class DeviceResources
    {
        struct DescriptorSizes
        {
            uint RTV;
            uint DSV;
            uint CBV_SRV_UAV;
        };

        DescriptorSizes _descriptorSizes;

        dxgi::Factory4Ptr _factory;

        d3d::DevicePtr _device;
        d3d::FencePtr _fence;

        d3d::CommandQueuePtr _commandQueue;
        d3d::CommandAllocatorPtr _commandAllocator;
        d3d::GraphicsCommandListPtr _commandList;

    public:
        DeviceResources();
        DeviceResources(HWND hwnd);
    };
}