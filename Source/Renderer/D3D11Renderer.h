#pragma once
#include "../Common.h"

#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "../Image/Image.h"
#include "../Image/ImageBMP.h"
#include "../Graphic/Buffer/ConstantBuffer.h"
#include "../Math/Math.h"
#include "../Graphic/Primitive/Primitive.h"

// Libraries
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

namespace Renderer{
	class D3D11Renderer : public IWindowRenderer
	{
	public:
		D3D11Renderer();
		virtual ~D3D11Renderer();

		void BeforeStart(HDC WindowDeviceContext, const bool isWindowed) override;
		void ClearWindow(const double deltaTime) override;
		void Update(const double deltaTime) override;
		void Render(const double deltaTime) override;

		void UploadTexture(Image::Image* image);

	private:
		IDXGISwapChain* SwapChain;
		ID3D11Device* Device;
		ID3D11DeviceContext* DeviceContext;
		ID3D11RenderTargetView* BackBuffer;
		ID3D11RasterizerState* RasterizerState;
		ID3D11SamplerState* SamplerState;
		ID3D11ShaderResourceView* TestTexture;

		void PrepareBuffers();
		void CreateShaders();

		void CreateVertexBuffer();
		void CreateConstantBuffer();

		void CreateDepthBuffer();

		// Buffers
		ID3D11Buffer* VertexBuffer;
		ID3D11Buffer* UniformBuffer;

		// Shaders
		ID3D11InputLayout* DefaultInputLayout;
		ID3D11VertexShader* RotationVertexShader;
		ID3D11PixelShader* SolidColorPixelShader;

		// Primitives
		Graphic::Primitive::CubePrimitive* Cube;
	};
}