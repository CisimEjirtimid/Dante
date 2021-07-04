#include "renderer.h"
#include "exception.h"

namespace dante::dx
{
    namespace constants
    {
        static constexpr uint SwapChainBufferCount = 2;
        static uint MultiSampleQualityLevels;
    }
    namespace
    {
        auto getWindowSize(HWND hwnd)
        {
            uint width = 0;
            uint height = 0;

            RECT rect;
            if (GetWindowRect(hwnd, &rect))
            {
                width = uint(rect.right - rect.left);
                height = uint(rect.bottom - rect.top);
            }

            return std::make_pair(width, height);
        }

        void enableDebugLayer()
        {
#if defined(_DEBUG)
            d3d::DebugPtr debug;
            ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)));
            debug->EnableDebugLayer();
#endif
        }

        dxgi::Factory4Ptr createFactory()
        {
            dxgi::Factory4Ptr factory;

            uint factoryCreationFlags = 0;
#if defined(_DEBUG)
            factoryCreationFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

            ThrowIfFailed(CreateDXGIFactory2(factoryCreationFlags, IID_PPV_ARGS(&factory)));

            return std::move(factory);
        }

        d3d::Device8Ptr createDevice()
        {
            d3d::Device8Ptr device;

            ThrowIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device)));

#if defined(_DEBUG)
            d3d::InfoQueuePtr infoQueue;
            ThrowIfFailed(device.As(&infoQueue));
            infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
            infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
            infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
#endif

            return std::move(device);
        }

        void checkFeatureSupport(d3d::Device8Ptr device)
        {
            D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS qualityLevels
            {
                .Format = DXGI_FORMAT_B8G8R8A8_UNORM,
                .SampleCount = 4,
                .Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE,
                .NumQualityLevels = 0
            };

            ThrowIfFailed(device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &qualityLevels, sizeof(qualityLevels)));

            if (qualityLevels.NumQualityLevels < 1)
                throw std::exception{ "Unexpected MSAA quality level!" };

            constants::MultiSampleQualityLevels = qualityLevels.NumQualityLevels;
        }

        void initDescriptorSizeMap(d3d::Device8Ptr device)
        {
            DescriptorSize[DescriptorType::RTV] = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
            DescriptorSize[DescriptorType::DSV] = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
            DescriptorSize[DescriptorType::CBV] = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        }

        d3d::FencePtr createFence(d3d::Device8Ptr device)
        {
            d3d::FencePtr fence;

            ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

            return std::move(fence);
        }

        d3d::CommandQueuePtr createCommandQueue(d3d::Device8Ptr device)
        {
            d3d::CommandQueuePtr commandQueue;

            D3D12_COMMAND_QUEUE_DESC queue
            {
                .Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
                .Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
            };
            ThrowIfFailed(device->CreateCommandQueue(&queue, IID_PPV_ARGS(&commandQueue)));

            return std::move(commandQueue);
        }

        d3d::CommandAllocatorPtr createCommandAllocator(d3d::Device8Ptr device)
        {
            d3d::CommandAllocatorPtr commandAllocator;

            ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));

            return std::move(commandAllocator);
        }

        d3d::GraphicsCommandListPtr createCommandList(d3d::Device8Ptr device, d3d::CommandAllocatorPtr commandAllocator)
        {
            d3d::GraphicsCommandListPtr commandList;

            ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));

            commandList->Close();

            return std::move(commandList);
        }

        dxgi::SwapChain4Ptr createSwapChain(dxgi::Factory4Ptr factory, d3d::CommandQueuePtr commandQueue, HWND hwnd)
        {
            auto [width, height] = getWindowSize(hwnd);

            dxgi::SwapChainPtr swapChain;

            DXGI_SWAP_CHAIN_DESC chain
            {
                .BufferDesc =
                {
                    .Width = width,
                    .Height = height,
                    .RefreshRate =
                    {
                        .Numerator = 60,
                        .Denominator = 1
                    },
                    .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
                    .ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
                    .Scaling = DXGI_MODE_SCALING_UNSPECIFIED
                },
                .SampleDesc =
                {
                    .Count = 4,
                    .Quality = constants::MultiSampleQualityLevels
                },
                .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
                .BufferCount = constants::SwapChainBufferCount,
                .OutputWindow = hwnd,
                .Windowed = true,
                .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
                .Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
            };

            // Note: Swap Chain uses Command Queue to perform flush
            ThrowIfFailed(factory->CreateSwapChain(commandQueue.Get(), &chain, swapChain.ReleaseAndGetAddressOf()));

            dxgi::SwapChain4Ptr swapChain4;
            ThrowIfFailed(swapChain.As(&swapChain4));

            return std::move(swapChain4);
        }

        d3d::DescriptorHeapPtr createRTVDescriptorHeap(d3d::Device8Ptr device)
        {
            d3d::DescriptorHeapPtr rtv;

            D3D12_DESCRIPTOR_HEAP_DESC heap
            {
                .Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
                .NumDescriptors = constants::SwapChainBufferCount,
                .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
                .NodeMask = 0
            };

            ThrowIfFailed(device->CreateDescriptorHeap(&heap, IID_PPV_ARGS(&rtv)));

            return std::move(rtv);
        }

        d3d::DescriptorHeapPtr createDSVDescriptorHeap(d3d::Device8Ptr device)
        {
            d3d::DescriptorHeapPtr dsv;

            D3D12_DESCRIPTOR_HEAP_DESC heap
            {
                .Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
                .NumDescriptors = 1,
                .Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
                .NodeMask = 0
            };

            ThrowIfFailed(device->CreateDescriptorHeap(&heap, IID_PPV_ARGS(&dsv)));

            return std::move(dsv);
        }
    }

    Renderer::Renderer()
        : _factory(nullptr)
        , _device(nullptr)
    {
    }

    Renderer::Renderer(HWND hwnd)
        : Renderer()
    {
        enableDebugLayer();

        _factory = createFactory();
        _device = createDevice();

        checkFeatureSupport(_device);
        initDescriptorSizeMap(_device);

        _fence = createFence(_device);
        _commandQueue = createCommandQueue(_device);
        _commandAllocator = createCommandAllocator(_device);
        _commandList = createCommandList(_device, _commandAllocator);

        _swapChain = createSwapChain(_factory, _commandQueue, hwnd);

        _rtvHeap = createRTVDescriptorHeap(_device);
        _dsvHeap = createDSVDescriptorHeap(_device);
    }
}