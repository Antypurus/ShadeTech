#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

using namespace Microsoft::WRL;

void init_d3d12();
const ComPtr<IDXGIFactory7>& get_factory();