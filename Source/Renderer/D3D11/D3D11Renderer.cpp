#include "D3D11Renderer.h"

using namespace Renderer;

D3D11Renderer::D3D11Renderer()
{
}

D3D11Renderer::~D3D11Renderer()
{
	// Clear materials
	for (auto material : Materials)
	{
		delete material;
	}

	Materials.clear();

	// Clear entities
	for (auto entity : GameEntitites)
	{
		delete entity;
	}

	GameEntitites.clear();

	SwapChain->SetFullscreenState(FALSE, NULL);

	UniformBuffer->Release();

	RasterizerState->Release();
	SwapChain->Release();
	BackBuffer->Release();
	Device->Release();
	DeviceContext->Release();
}

void D3D11Renderer::BeforeStart(HDC WindowDeviceContext, const bool isWindowed)
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
	swapChainDescription.SampleDesc.Count = 1; // multi-samples
	swapChainDescription.Windowed = isWindowed;
	swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDescription.BufferDesc.Width = windowRectangle.right;
	swapChainDescription.BufferDesc.Height = windowRectangle.bottom;

	UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	flags |= D3D11_CREATE_DEVICE_DEBUG;

	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		flags,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDescription,
		&SwapChain,
		&Device,
		NULL,
		&DeviceContext);

	HRESULT result = S_OK;

	// Create and set viewport
	D3D11_VIEWPORT viewport = { 0, 0, (float)Parameters.Width, (float)Parameters.Height, 0.0f, 1.0f };
	DeviceContext->RSSetViewports(1, &viewport);

	PrepareBuffers();

	{
		// Cube entity
		Core::GameEntity* cube = new Game::Entities::CubeEntity("Cube1");
		GameEntitites.push_back(cube);
	}

	{
		for (auto entity : GameEntitites)
		{
			entity->SetRenderer(this);
			entity->OnLoad();

			std::vector<Graphic::Vertex>* verticies = entity->GetVerticies();

			CreateVertexBufferForEntity(entity);

			D3D11Material* material = static_cast<D3D11Material*>(entity->GetMaterial());

			D3D11_MAPPED_SUBRESOURCE mappedSubResource;

			result = DeviceContext->Map(material->VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubResource);
			memcpy(mappedSubResource.pData, &verticies->at(0), entity->GetVertexBufferWidth());
			DeviceContext->Unmap(material->VertexBuffer, NULL);

			assert(!(FAILED(result)));

			CreateShaderForEntity(entity);
		}
	}

	D3D11_RASTERIZER_DESC rasterizerDescription;
	ZeroMemory(&rasterizerDescription, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDescription.FillMode = D3D11_FILL_SOLID;
	rasterizerDescription.CullMode = D3D11_CULL_NONE;

	Device->CreateRasterizerState(&rasterizerDescription, &RasterizerState);
}

void D3D11Renderer::CreateDepthBuffer()
{

}

void D3D11Renderer::ClearWindow(const double deltaTime)
{
	const FLOAT color[4] = { sin((FLOAT)deltaTime) * 0.5f + 0.5f, cos((FLOAT)deltaTime) * 0.5f + 0.5f, 0.0f, 1.0f };
	DeviceContext->ClearRenderTargetView(BackBuffer, color);
}

void D3D11Renderer::Update(const double deltaTime)
{
	HRESULT result = S_OK;

	// Modify constant values
	{
		static unsigned __int32 frameCounter = 0;

		D3D11_MAPPED_SUBRESOURCE constantBufferMappedResource;
		result = DeviceContext->Map(UniformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantBufferMappedResource);
		Graphic::Buffer::ConstantBuffer* mappedConstantBuffer = (Graphic::Buffer::ConstantBuffer*)constantBufferMappedResource.pData;

		const glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), (float)Parameters.Width / (float)Parameters.Height, 0.1f, 100.0f);
		const glm::mat4 viewMatrix = glm::lookAt(glm::vec3(4, 3, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		const glm::mat4 modelMatrix = glm::mat4(1.0f);

		const glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

		mappedConstantBuffer->ModelViewProjectionMatrix = Math::Matrix4x4((const float*)&mvpMatrix);
		mappedConstantBuffer->FrameNumber = frameCounter;

		DeviceContext->Unmap(UniformBuffer, 0);

		frameCounter++;
	}

	assert(!(FAILED(result)));

	for (auto entity : GameEntitites)
	{
		entity->OnUpdate(deltaTime);
	}
}

void D3D11Renderer::Render(const double deltaTime)
{
	HRESULT result = S_OK;

	UINT stride = 0;
	UINT offset = 0;

	for (auto entity : GameEntitites)
	{
		Renderer::D3D11Material* material = static_cast<Renderer::D3D11Material*>(entity->GetMaterial());

		DeviceContext->VSSetShader(material->VertexShader, nullptr, 0);
		DeviceContext->PSSetShader(material->PixelShader, nullptr, 0);

		stride = entity->GetVertexBufferStride();

		DeviceContext->IASetVertexBuffers(0, 1, &material->VertexBuffer, &stride, &offset);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		DeviceContext->RSSetState(RasterizerState);

		DeviceContext->PSSetSamplers(0, 1, &material->SamplerState);
		DeviceContext->PSSetShaderResources(0, 1, &material->DiffuseTexture);

		// Draw
		DeviceContext->Draw(entity->GetVerticies()->size(), 0);

		// Swap
		result = SwapChain->Present(0, 0);

		assert(!(FAILED(result)));
	}
}

void D3D11Renderer::UploadTexture(Core::GameEntity* entity, Image::Image* image)
{
	HRESULT result;

	Renderer::D3D11Material* material = static_cast<Renderer::D3D11Material*>(entity->GetMaterial());

	image->Load();

	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = image->GetWidth();
	textureDesc.Height = image->GetHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	unsigned int bytesPerPixel = 24 / 8;

	D3D11_SUBRESOURCE_DATA textureSubResource;
	textureSubResource.pSysMem = image->GetDataPointer();
	textureSubResource.SysMemPitch = static_cast<UINT>(bytesPerPixel * image->GetWidth());
	textureSubResource.SysMemSlicePitch = static_cast<UINT>(bytesPerPixel * image->GetHeight() * image->GetWidth());

	ID3D11Texture2D* tex;
	result = Device->CreateTexture2D(&textureDesc, &textureSubResource, &tex);

	assert(!(FAILED(result)));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	srvDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

	result = Device->CreateShaderResourceView(tex, &srvDesc, &material->DiffuseTexture);

	assert(!(FAILED(result)));

	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = Device->CreateSamplerState(&samplerDesc, &material->SamplerState);

	image->Free();

	assert(!(FAILED(result)));
}

Renderer::Material* D3D11Renderer::CreateMaterial()
{
	Renderer::D3D11Material* material = new Renderer::D3D11Material();

	// Keep track on every material which is created by renderer
	Materials.push_back(material);

	return material;
}

void D3D11Renderer::PrepareBuffers()
{
	CreateBackBuffer();

	CreateConstantBuffer();
}

void D3D11Renderer::CreateShaderForEntity(Core::GameEntity* entity)
{
	HRESULT result = S_OK;

	Renderer::Material* material = entity->GetMaterial();

	std::string vertexShaderName = material->VertexShader + ".cso";
	std::string pixelShaderName = material->PixelShader + ".cso";

	D3D11Material* d3d11Material = static_cast<D3D11Material*>(entity->GetMaterial());

	// Vertex
	FileSystem::File vertexShaderFile(vertexShaderName.c_str());
	vertexShaderFile.Load();

	std::vector<char> vertexShaderBytes = vertexShaderFile.GetBinaryContent();
	size_t vertexShaderBytesSize = vertexShaderBytes.size() * sizeof(char);
	const char* vertexShaderBytesPointer = &vertexShaderBytes[0];

	result = Device->CreateVertexShader(vertexShaderBytesPointer, vertexShaderBytesSize, nullptr, &d3d11Material->VertexShader);

	// Pixel shader
	FileSystem::File pixelShaderFile(pixelShaderName.c_str());
	pixelShaderFile.Load();

	std::vector<char> pixelShaderBytes = pixelShaderFile.GetBinaryContent();
	size_t pixelShaderBytesSize = pixelShaderBytes.size() * sizeof(char);
	const char* pixelShaderBytesPointer = &pixelShaderBytes[0];

	result = Device->CreatePixelShader(pixelShaderBytesPointer, pixelShaderBytesSize, nullptr, &d3d11Material->PixelShader);

	// Input layout
	const D3D11_INPUT_ELEMENT_DESC inputElementDescription[] =
	{
		{ "VERTEX_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 3 * 4
		{ "VERTEX_COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 4 * 4
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 2 * 4
	};

	result = Device->CreateInputLayout(inputElementDescription, ARRAYSIZE(inputElementDescription), vertexShaderBytesPointer, vertexShaderBytesSize, &d3d11Material->InputLayout);

	DeviceContext->IASetInputLayout(d3d11Material->InputLayout);

	assert(!(FAILED(result)));
}

void D3D11Renderer::CreateBackBuffer()
{
	HRESULT result = S_OK;

	ID3D11Texture2D *pBackBuffer;

	result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	result = Device->CreateRenderTargetView(pBackBuffer, NULL, &BackBuffer);

	pBackBuffer->Release();

	DeviceContext->OMSetRenderTargets(1, &BackBuffer, NULL);

	assert(!(FAILED(result)));
}

void D3D11Renderer::CreateVertexBufferForEntity(Core::GameEntity* entity)
{
	HRESULT result = S_OK;

	Renderer::D3D11Material* material = static_cast<Renderer::D3D11Material*>(entity->GetMaterial());

	D3D11_BUFFER_DESC bufferDescription;
	ZeroMemory(&bufferDescription, sizeof(bufferDescription));

	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.ByteWidth = entity->GetVertexBufferWidth();
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	result = Device->CreateBuffer(&bufferDescription, NULL, &material->VertexBuffer);

	assert(!(FAILED(result)));
}

void D3D11Renderer::CreateConstantBuffer()
{
	HRESULT result = S_OK;

	Graphic::Buffer::ConstantBuffer buffer;

	buffer.ModelViewProjectionMatrix = { };
	buffer.FrameNumber = 0;

	D3D11_BUFFER_DESC bufferDescription;
	ZeroMemory(&bufferDescription, sizeof(bufferDescription));

	bufferDescription.ByteWidth = sizeof(Graphic::Buffer::ConstantBuffer);
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA subResourceData;
	ZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = &buffer;

	result = Device->CreateBuffer(&bufferDescription, &subResourceData, &UniformBuffer);

	DeviceContext->VSSetConstantBuffers(0, 1, &UniformBuffer);

	assert(!(FAILED(result)));
}
