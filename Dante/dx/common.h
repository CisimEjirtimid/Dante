#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

namespace dante::dx
{
    using uint = unsigned int;

    namespace wrl = Microsoft::WRL;

    namespace dxgi
    {
        using Factory4Ptr = wrl::ComPtr<IDXGIFactory4>;
    }

    namespace d3d
    {
        using DebugPtr = wrl::ComPtr<ID3D12Debug>;

        using DevicePtr = wrl::ComPtr<ID3D12Device>;

        using CommandQueuePtr = wrl::ComPtr<ID3D12CommandQueue>;
        using CommandListPtr = wrl::ComPtr<ID3D12CommandList>;
        using GraphicsCommandListPtr = wrl::ComPtr<ID3D12GraphicsCommandList>;
        using CommandAllocatorPtr = wrl::ComPtr<ID3D12CommandAllocator>;

        using FencePtr = wrl::ComPtr<ID3D12Fence>;
    }
}