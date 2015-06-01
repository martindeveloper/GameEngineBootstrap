#pragma once
#include "Headers.h"

#include <d3d11.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

namespace RendererBootstrap{
	class D3D11Renderer : public IWindowRenderer
	{
	private:
		IDXGISwapChain *SwapChain;
		ID3D11Device *Device;
		ID3D11DeviceContext *DeviceContext;
		ID3D11RenderTargetView *BackBuffer;

	public:
		D3D11Renderer();
		void BeforeStart(HDC WindowDeviceContext, bool isWindowed) override;
		void ClearWindow(double deltaTime) override;
		void Render(double deltaTime) override;
		virtual ~D3D11Renderer();
	};
}