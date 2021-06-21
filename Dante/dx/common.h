#pragma once
#include <d3d12.h>
#include <wrl.h>

namespace dante::dx
{
    namespace wrl = Microsoft::WRL;

    using DevicePtr = wrl::ComPtr<ID3D12Device>;

    using CommandQueuePtr = wrl::ComPtr<ID3D12CommandQueue>;
    using CommandListPtr = wrl::ComPtr<ID3D12CommandList>;
    using CommandAllocatorPtr = wrl::ComPtr<ID3D12CommandAllocator>;

    using FencePtr = wrl::ComPtr<ID3D12Fence>;
}