#pragma once
#include "Headers.h"

#include <d3d12.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d12.lib")

namespace RendererBootstrap {
	class D3D12Renderer : public IWindowRenderer
	{
	private:
		IDXGISwapChain *SwapChain;
		ID3D11Device *Device;
		ID3D11DeviceContext *DeviceContext;
		ID3D11RenderTargetView *BackBuffer;

	public:
		D3D12Renderer();
		void BeforeStart(HDC WindowDeviceContext, bool isWindowed) override;
		void ClearWindow(double deltaTime) override;
		void Update(double deltaTime) override;
		void Render(double deltaTime) override;

		// Populate Command Lists
		// Wait for GPU
		// Set resource barrier

		virtual ~D3D12Renderer();
	};
}