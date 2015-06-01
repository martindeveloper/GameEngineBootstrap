#include "D3D11Renderer.h"

using namespace RendererBootstrap;

D3D11Renderer::D3D11Renderer()
{
}

D3D11Renderer::~D3D11Renderer()
{
	SwapChain->SetFullscreenState(FALSE, NULL);

	SwapChain->Release();
	BackBuffer->Release();
	Device->Release();
	DeviceContext->Release();
}

void D3D11Renderer::BeforeStart(HDC WindowDeviceContext, bool isWindowed)
{
	DXGI_SWAP_CHAIN_DESC swapChainDescription;

	SecureZeroMemory(&swapChainDescription, sizeof(DXGI_SWAP_CHAIN_DESC));

	HWND windowHandle = WindowFromDC(WindowDeviceContext);

	RECT windowRectangle = { 0 };
	GetWindowRect(windowHandle, &windowRectangle);

	swapChainDescription.BufferCount = 1;
	swapChainDescription.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescription.OutputWindow = windowHandle;
	swapChainDescription.SampleDesc.Count = 4; // multi-samples
	swapChainDescription.Windowed = isWindowed;
	swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDescription.BufferDesc.Width = windowRectangle.right;
	swapChainDescription.BufferDesc.Height = windowRectangle.bottom;

	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDescription,
		&SwapChain,
		&Device,
		NULL,
		&DeviceContext);

	ID3D11Texture2D *pBackBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	Device->CreateRenderTargetView(pBackBuffer, NULL, &BackBuffer);
	pBackBuffer->Release();

	DeviceContext->OMSetRenderTargets(1, &BackBuffer, NULL);
}

void D3D11Renderer::ClearWindow(double deltaTime)
{
	const FLOAT color[4] = { sin(deltaTime) * 0.5f + 0.5f, cos(deltaTime) * 0.5f + 0.5f, 0.0f, 1.0f };
	DeviceContext->ClearRenderTargetView(BackBuffer, color);
}

void D3D11Renderer::Render(double deltaTime)
{
	SwapChain->Present(0, 0);
}