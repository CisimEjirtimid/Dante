#include "device_resources.h"
#include <exception>

namespace dante::dx
{
    DeviceResources::DeviceResources()
        : _descriptorSizes(DescriptorSizes{ 0, 0, 0 })
        , _factory(nullptr)
        , _device(nullptr)
    {
    }

    DeviceResources::DeviceResources(HWND hwnd)
        : DeviceResources()
    {
        HRESULT hr;

#if defined(_DEBUG)
        d3d::DebugPtr debug;
        hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
        debug->EnableDebugLayer();
#endif

        // TODO: add return/throw if failed for hr error handling
        hr = CreateDXGIFactory1(IID_PPV_ARGS(&_factory));
        hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&_device));
        hr = _device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));

        _descriptorSizes.RTV = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        _descriptorSizes.DSV = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
        _descriptorSizes.CBV_SRV_UAV = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS qualityLevels
        {
            .Format = DXGI_FORMAT_B8G8R8A8_UNORM,
            .SampleCount = 4,
            .Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE,
            .NumQualityLevels = 0
        };
        hr = _device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &qualityLevels, sizeof(qualityLevels));

        if (qualityLevels.NumQualityLevels < 1)
            throw std::exception{ "Unexpected MSAA quality level!" };

        D3D12_COMMAND_QUEUE_DESC queue
        {
            .Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
            .Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
        };
        hr = _device->CreateCommandQueue(&queue, IID_PPV_ARGS(&_commandQueue));
        hr = _device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_commandAllocator));
        hr = _device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocator.Get(), nullptr, IID_PPV_ARGS(&_commandList));

        _commandList->Close();
    }
}