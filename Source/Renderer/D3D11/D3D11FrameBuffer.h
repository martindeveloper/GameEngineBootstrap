#if RENDERER_DIRECTX11

#pragma once

#include "../../Common.h"
#include "D3D11Renderer.h"
#include "../IFrameBuffer.h"

namespace Renderer
{
	class D3D11Renderer;

	class D3D11FrameBuffer : IFrameBuffer
	{
	public:
		ID3D11RenderTargetView* BackBuffer;
		ID3D11RenderTargetView* FrontBuffer;

		D3D11FrameBuffer(Renderer::D3D11Renderer* renderer);
		virtual ~D3D11FrameBuffer();

		void Create() override;
		void Bind() override;
		void Unbind() override;
		void Draw() override;

	private:
		Renderer::D3D11Renderer* Renderer;

		void CreateBuffer(ID3D11RenderTargetView** buffer);
	};
}

#endif
