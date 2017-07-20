#if RENDERER_DIRECTX11

#pragma once

#include "../../Common.h"

#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "../../Image/Image.h"
#include "../../Image/ImageBMP.h"

#include "../../Graphic/Buffer/ConstantBuffer.h"

#include "../../Math/Math.h"

#include "D3D11FrameBuffer.h"
#include "D3D11Material.h"

#include "../../Graphic/Vertex.h"
#include "../../Core/GameEntity.h"
#include "../../Game/Entities/CubeEntity.h"

#include "../../Game/Components/RendererComponent.h"

namespace Renderer
{
	class D3D11FrameBuffer;

	class D3D11Renderer : public IWindowRenderer
	{
	public:
		IDXGISwapChain* SwapChain;
		ID3D11Device* Device;
		ID3D11DeviceContext* DeviceContext;
		ID3D11DepthStencilState* DepthStencilState;
		ID3D11DepthStencilView* DepthStencilView;

		D3D11Renderer();
		virtual ~D3D11Renderer();

		void BeforeStart(HDC WindowDeviceContext, const bool isWindowed) override;
		void ClearWindow(const double deltaTime) override;
		void Update(const double deltaTime) override;
		void Render(const double deltaTime) override;

		Renderer::Material* CreateMaterial() override;
		void UploadTexture(Core::GameEntity* entity, Image::Image* image) override;

	private:
		D3D11FrameBuffer* FrameBuffer;
		ID3D11RasterizerState* RasterizerState;

		// Global buffers
		ID3D11Buffer* UniformBuffer;
		ID3D11Buffer* TransformBuffer;

		// Materials
		std::vector<Renderer::D3D11Material*> Materials;

		void PrepareBuffers() override;

		template<typename T>
		void CreateConstantBuffer(ID3D11Buffer** targetBuffer);

		void CreateShaderForEntity(Core::GameEntity* entity) override;

		void CreateVertexBufferForEntity(Core::GameEntity* entity);
		void CreateDepthBuffer();
	};
}

#endif
