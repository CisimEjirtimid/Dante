#pragma once
#include "common.h"
#include <windef.h>
#include <map>

namespace dante::dx
{
    enum class DescriptorType : int8_t
    {
        RTV = 0,
        DSV = 1,
        CBV = 2,
        SRV = 2,
        UAV = 2
    };

    static std::map<DescriptorType, uint> DescriptorSize;

    class Renderer
    {
        dxgi::Factory4Ptr _factory;
        dxgi::SwapChain4Ptr _swapChain;

        d3d::Device8Ptr _device;

        d3d::FencePtr _fence;
        d3d::CommandQueuePtr _commandQueue;
        d3d::CommandAllocatorPtr _commandAllocator;
        d3d::GraphicsCommandListPtr _commandList;

        d3d::DescriptorHeapPtr _rtvHeap;
        d3d::DescriptorHeapPtr _dsvHeap;

    public:
        Renderer();
        Renderer(HWND hwnd);
    };
}