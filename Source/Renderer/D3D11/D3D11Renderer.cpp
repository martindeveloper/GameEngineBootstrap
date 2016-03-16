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

	delete FrameBuffer;

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
	swapChainDescription.SampleDesc.Quality = 0;
	swapChainDescription.Windowed = isWindowed;
	swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDescription.BufferDesc.Width = windowRectangle.right;
	swapChainDescription.BufferDesc.Height = windowRectangle.bottom;

	Parameters = { windowRectangle.right, windowRectangle.bottom };

	UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	flags |= D3D11_CREATE_DEVICE_DEBUG; 
	flags |= D3D11_RLDO_DETAIL;

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

			CreateConstantBuffer<Graphic::Buffer::TransformBuffer>(&material->TransformBuffer);
		}
	}

	D3D11_RASTERIZER_DESC rasterizerDescription;
	ZeroMemory(&rasterizerDescription, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDescription.AntialiasedLineEnable = false;
	rasterizerDescription.CullMode = D3D11_CULL_BACK;
	rasterizerDescription.DepthBias = 0;
	rasterizerDescription.DepthBiasClamp = 0.0f;
	rasterizerDescription.DepthClipEnable = true;
	rasterizerDescription.FillMode = D3D11_FILL_SOLID;
	rasterizerDescription.FrontCounterClockwise = false;
	rasterizerDescription.MultisampleEnable = false;
	rasterizerDescription.ScissorEnable = false;
	rasterizerDescription.SlopeScaledDepthBias = 0.0f;

	Device->CreateRasterizerState(&rasterizerDescription, &RasterizerState);

	PrepareBuffers();
}

void D3D11Renderer::CreateDepthBuffer()
{
	HRESULT result = S_OK;

	ID3D11Texture2D* depthStencilBuffer = NULL;
	D3D11_TEXTURE2D_DESC depthStencilDescription;
	ZeroMemory(&depthStencilDescription, sizeof(D3D11_TEXTURE2D_DESC));

	D3D11_RENDER_TARGET_VIEW_DESC backBufferDescription;
	ZeroMemory(&backBufferDescription, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

	FrameBuffer->BackBuffer->GetDesc(&backBufferDescription);

	depthStencilDescription.Width = Parameters.Width;
	depthStencilDescription.Height = Parameters.Height;
	depthStencilDescription.MipLevels = 1;
	depthStencilDescription.ArraySize = 1;
	depthStencilDescription.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDescription.SampleDesc.Count = 1;
	depthStencilDescription.SampleDesc.Quality = 0;
	depthStencilDescription.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	depthStencilDescription.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	depthStencilDescription.CPUAccessFlags = 0;
	depthStencilDescription.MiscFlags = 0;

	result = Device->CreateTexture2D(&depthStencilDescription, NULL, &depthStencilBuffer);

	assert(!(FAILED(result)));

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	ID3D11DepthStencilState* pDSState;
	result = Device->CreateDepthStencilState(&dsDesc, &pDSState);

	assert(!(FAILED(result)));

	// Bind depth stencil state
	DeviceContext->OMSetDepthStencilState(pDSState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	descDSV.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	// Create the depth stencil view
	result = Device->CreateDepthStencilView(depthStencilBuffer, &descDSV, &DepthStencilView);

	assert(!(FAILED(result)));
}

void D3D11Renderer::ClearWindow(const double deltaTime)
{
	const FLOAT color[4] = { sin((FLOAT)deltaTime) * 0.5f + 0.5f, cos((FLOAT)deltaTime) * 0.5f + 0.5f, 0.0f, 1.0f };

	DeviceContext->ClearRenderTargetView(FrameBuffer->BackBuffer, color);
	//DeviceContext->ClearRenderTargetView(FrameBuffer->FrontBuffer, color);

	DeviceContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void D3D11Renderer::Update(const double deltaTime)
{
	HRESULT result = S_OK;

	// Modify constant values
	{
		static uint32 frameCounter = 0;

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
		Renderer::D3D11Material* material = static_cast<Renderer::D3D11Material*>(entity->GetMaterial());

		entity->OnUpdate(deltaTime);

		// Pass new transform values to shader
		D3D11_MAPPED_SUBRESOURCE transformBufferMappedResource;
		result = DeviceContext->Map(material->TransformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &transformBufferMappedResource);
		Graphic::Buffer::TransformBuffer* mappedTransformBuffer = (Graphic::Buffer::TransformBuffer*)transformBufferMappedResource.pData;
		
		mappedTransformBuffer->Position = material->Transform.Position;
		mappedTransformBuffer->Scale = material->Transform.Scale;

		DeviceContext->Unmap(material->TransformBuffer, 0);
	}
}

void D3D11Renderer::Render(const double deltaTime)
{
	HRESULT result = S_OK;

	UINT stride = 0;
	UINT offset = 0;

	FrameBuffer->Bind();

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
	}

	FrameBuffer->Unbind();

	// Swap
	result = SwapChain->Present(0, 0);

	assert(!(FAILED(result)));
}

void D3D11Renderer::UploadTexture(Core::GameEntity* entity, Image::Image* image)
{
	HRESULT result;

	Renderer::D3D11Material* material = static_cast<Renderer::D3D11Material*>(entity->GetMaterial());

	image->Load();

	DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	textureDesc.Width = image->GetWidth();
	textureDesc.Height = image->GetHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	constexpr uint32 bytesPerPixel = 32 / 8; // NOTE(martin.pernica): 34 is hardcoded value from BMP header. 
	const uint32 pitch = bytesPerPixel * image->GetWidth();

	D3D11_SUBRESOURCE_DATA textureSubResource;
	ZeroMemory(&textureSubResource, sizeof(D3D11_SUBRESOURCE_DATA));

	textureSubResource.pSysMem = image->GetDataPointer();
	textureSubResource.SysMemPitch = pitch;

	ID3D11Texture2D* tex;
	result = Device->CreateTexture2D(&textureDesc, &textureSubResource, &tex);

	assert(!(FAILED(result)));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	srvDesc.Format = format;

	result = Device->CreateShaderResourceView(tex, &srvDesc, &material->DiffuseTexture);

	assert(!(FAILED(result)));

	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
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
	FrameBuffer = new D3D11FrameBuffer(this);
	FrameBuffer->Create();

	CreateDepthBuffer();

	CreateConstantBuffer<Graphic::Buffer::ConstantBuffer>(&UniformBuffer);
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
		{ "VERTEX_COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 4 * 4
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 2 * 4
	};

	result = Device->CreateInputLayout(inputElementDescription, ARRAYSIZE(inputElementDescription), vertexShaderBytesPointer, vertexShaderBytesSize, &d3d11Material->InputLayout);

	DeviceContext->IASetInputLayout(d3d11Material->InputLayout);

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

template<typename T>
void D3D11Renderer::CreateConstantBuffer(ID3D11Buffer** targetBuffer)
{
	HRESULT result = S_OK;

	T buffer;
	ZeroMemory(&buffer, sizeof(T));

	D3D11_BUFFER_DESC bufferDescription;
	ZeroMemory(&bufferDescription, sizeof(bufferDescription));

	bufferDescription.ByteWidth = sizeof(T);
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA subResourceData;
	ZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = &buffer;

	result = Device->CreateBuffer(&bufferDescription, &subResourceData, targetBuffer);

	DeviceContext->VSSetConstantBuffers(0, 1, targetBuffer);

	assert(!(FAILED(result)));
}
