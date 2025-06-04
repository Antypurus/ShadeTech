#include "sdk.h"

#include <core/types.h>

#include <vector>

ComPtr<IDXGIFactory7> g_dxgi_factory;

void init_d3d12()
{
    CreateDXGIFactory2(0, IID_PPV_ARGS(&g_dxgi_factory));

    u32 count = 0;
    IDXGIAdapter* adapter;
    std::vector<ComPtr<IDXGIAdapter>> adapters;
    while(get_factory()->EnumAdapters(count++, &adapter) != DXGI_ERROR_NOT_FOUND)
    {
        adapters.push_back(adapter);
        adapter = nullptr;
    }
}

const ComPtr<IDXGIFactory7>& get_factory()
{
    return g_dxgi_factory;
}