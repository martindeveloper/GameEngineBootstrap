#pragma once

#include "../../Common.h"
#include "../Material.h"

#include <d3d11.h>

namespace Renderer
{
	class D3D11Material : public Renderer::Material
	{
	public:
		virtual ~D3D11Material()
		{
			VertexShader->Release();
			PixelShader->Release();

			InputLayout->Release();

			SamplerState->Release();
			DiffuseTexture->Release();

			VertexBuffer->Release();
		}

		ID3D11Buffer* TransformBuffer;
		ID3D11Buffer* VertexBuffer;

		ID3D11VertexShader* VertexShader;
		ID3D11PixelShader* PixelShader;

		ID3D11SamplerState* SamplerState;
		ID3D11ShaderResourceView* DiffuseTexture;

		ID3D11InputLayout* InputLayout;
	};
}
