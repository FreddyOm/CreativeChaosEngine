#pragma once

// D3D11 / DXGI Header
#include <d3d11.h>
#include <dxgidebug.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "dxgi.lib")
//#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")

// WRL
#include <wrl.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;