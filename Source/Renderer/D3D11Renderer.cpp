#include "D3D11Renderer.h"

using namespace Renderer;

D3D11Renderer::D3D11Renderer()
{
}

D3D11Renderer::~D3D11Renderer()
{
	delete Cube;

	SwapChain->SetFullscreenState(FALSE, NULL);

	// Release shaders
	RotationVertexShader->Release();
	SolidColorPixelShader->Release();
	//

	VertexBuffer->Release();
	UniformBuffer->Release();

	DefaultInputLayout->Release();
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

	// Cube primitive
	Cube = new Graphic::Primitive::CubePrimitive(Graphic::ColorRGBA::Red());

	// Must be AFTER Cube is created, we need to have Cube ready to fill ByteWidth of Vertex Buffer
	PrepareBuffers();

	FileSystem::File sourceFile("lenaColor512.bmp");
	Image::ImageBMP sourceBitmap(&sourceFile);

	UploadTexture((Image::Image*)&sourceBitmap);

	D3D11_MAPPED_SUBRESOURCE mappedSubResource;

	result = DeviceContext->Map(VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubResource);
	memcpy(mappedSubResource.pData, Cube->Verticies, sizeof(Cube->Verticies));
	DeviceContext->Unmap(VertexBuffer, NULL);

	assert(!(FAILED(result)));

	// Create and set viewport
	D3D11_VIEWPORT viewport = {0, 0, (float)Parameters.Width, (float)Parameters.Height, 0.0f, 1.0f };
	DeviceContext->RSSetViewports(1, &viewport);

	CreateShaders();

	D3D11_RASTERIZER_DESC rasterizerDescription;
	ZeroMemory(&rasterizerDescription, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDescription.FillMode = D3D11_FILL_SOLID;
	rasterizerDescription.CullMode = D3D11_CULL_NONE;

	Device->CreateRasterizerState(&rasterizerDescription, &RasterizerState);
}

void D3D11Renderer::CreateDepthBuffer()
{
	/*
	D3D11_TEXTURE2D_DESC depthStencil_DESCRIPTOR;
	ZeroMemory(&depthStencil_DESCRIPTOR, sizeof(D3D11_TEXTURE2D_DESC));
	depthStencil_DESCRIPTOR.Width = Parameters.Width;
	depthStencil_DESCRIPTOR.Height = Parameters.Height;
	depthStencil_DESCRIPTOR.MipLevels = 1;
	depthStencil_DESCRIPTOR.ArraySize = 1;
	depthStencil_DESCRIPTOR.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencil_DESCRIPTOR.SampleDesc.Count = 4;
	depthStencil_DESCRIPTOR.SampleDesc.Quality = m4xMsaaQuality - 1;
	depthStencil_DESCRIPTOR.Usage = D3D11_USAGE_DEFAULT;
	depthStencil_DESCRIPTOR.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencil_DESCRIPTOR.CPUAccessFlags = 0;
	depthStencil_DESCRIPTOR.MiscFlags = 0;
	ID3D11Texture2D* depthStencilBuffer;
	result = m_Device->CreateTexture2D(&depthStencil_DESCRIPTOR, 0, &depthStencilBuffer);
	if (FAILED(result))
		return false;
	D3D11_DEPTH_STENCIL_DESC DepthStencilState_DESCRIPTOR;
	ZeroMemory(&DepthStencilState_DESCRIPTOR, sizeof(D3D11_DEPTH_STENCIL_DESC));
	DepthStencilState_DESCRIPTOR.DepthEnable = true;
	DepthStencilState_DESCRIPTOR.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilState_DESCRIPTOR.DepthFunc = D3D11_COMPARISON_LESS;
	DepthStencilState_DESCRIPTOR.StencilEnable = true;
	DepthStencilState_DESCRIPTOR.StencilReadMask = 0xFF;
	DepthStencilState_DESCRIPTOR.StencilWriteMask = 0xFF; // Stencil Operations on Front facing pixels
	DepthStencilState_DESCRIPTOR.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; 
	DepthStencilState_DESCRIPTOR.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR; 
	DepthStencilState_DESCRIPTOR.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; 
	DepthStencilState_DESCRIPTOR.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS; // Stencil operations on Back facint pixels
	DepthStencilState_DESCRIPTOR.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; 
	DepthStencilState_DESCRIPTOR.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR; 
	DepthStencilState_DESCRIPTOR.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP; 
	DepthStencilState_DESCRIPTOR.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS; 
	result = m_Device->CreateDepthStencilState(&DepthStencilState_DESCRIPTOR, &m_DepthStencilState); 
	if (FAILED(result)) return false; 

	m_Context->OMSetDepthStencilState(m_DepthStencilState, 1); 
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilView_DESCRIPTOR; 
	ZeroMemory(&DepthStencilView_DESCRIPTOR, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC)); 
	DepthStencilView_DESCRIPTOR.Format	= depthStencil_DESCRIPTOR.Format; 
	DepthStencilView_DESCRIPTOR.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; 
	DepthStencilView_DESCRIPTOR.Texture2D.MipSlice = 0; 
	result = m_Device->CreateDepthStencilView(depthStencilBuffer, &DepthStencilView_DESCRIPTOR, &m_DepthStencilView); 

	if (FAILED(result)) return false;
	*/
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
}

void D3D11Renderer::Render(const double deltaTime)
{
	HRESULT result = S_OK;

	UINT stride = sizeof(Graphic::Vertex);
	UINT offset = 0;

	DeviceContext->VSSetShader(RotationVertexShader, nullptr, 0);
	DeviceContext->PSSetShader(SolidColorPixelShader, nullptr, 0);

	DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DeviceContext->RSSetState(RasterizerState);

	DeviceContext->PSSetSamplers(0, 1, &SamplerState);
	DeviceContext->PSSetShaderResources(0, 1, &TestTexture);

	// Draw
	DeviceContext->Draw(Cube->DrawCount, 0);

	// Swap
	result = SwapChain->Present(0, 0);

	assert(!(FAILED(result)));
}

void D3D11Renderer::UploadTexture(Image::Image* image)
{
	HRESULT result;

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

	result = Device->CreateShaderResourceView(tex, &srvDesc, &TestTexture);

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
	result = Device->CreateSamplerState(&samplerDesc, &SamplerState);
	assert(!(FAILED(result)));

	image->Free();
}

void D3D11Renderer::PrepareBuffers()
{
	CreateVertexBuffer();

	CreateConstantBuffer();
}

void D3D11Renderer::CreateShaders()
{
	HRESULT result = S_OK;

	// Vertex shader
	FileSystem::File vertexShaderFile("RotationVertex.cso");
	vertexShaderFile.Load();

	std::vector<char> vertexShaderBytes = vertexShaderFile.GetBinaryContent();
	size_t vertexShaderBytesSize = vertexShaderBytes.size() * sizeof(char);
	const char* vertexShaderBytesPointer = &vertexShaderBytes[0];

	result = Device->CreateVertexShader(vertexShaderBytesPointer, vertexShaderBytesSize, nullptr, &RotationVertexShader);

	// Pixel shader
	FileSystem::File pixelShaderFile("SolidColorPixel.cso");
	pixelShaderFile.Load();

	std::vector<char> pixelShaderBytes = pixelShaderFile.GetBinaryContent();
	size_t pixelShaderBytesSize = pixelShaderBytes.size() * sizeof(char);
	const char* pixelShaderBytesPointer = &pixelShaderBytes[0];

	result = Device->CreatePixelShader(pixelShaderBytesPointer, pixelShaderBytesSize, nullptr, &SolidColorPixelShader);

	// Input layout
	const D3D11_INPUT_ELEMENT_DESC inputElementDescription[] =
	{
		{ "VERTEX_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 3 * 4
		{ "VERTEX_COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 4 * 4
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 2 * 4
	};

	result = Device->CreateInputLayout(inputElementDescription, ARRAYSIZE(inputElementDescription), vertexShaderBytesPointer, vertexShaderBytesSize, &DefaultInputLayout);

	DeviceContext->IASetInputLayout(DefaultInputLayout);

	assert(!(FAILED(result)));
}

void D3D11Renderer::CreateVertexBuffer()
{
	HRESULT result = S_OK;

	ID3D11Texture2D *pBackBuffer;

	result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	result = Device->CreateRenderTargetView(pBackBuffer, NULL, &BackBuffer);

	pBackBuffer->Release();

	DeviceContext->OMSetRenderTargets(1, &BackBuffer, NULL);

	D3D11_BUFFER_DESC bufferDescription;
	ZeroMemory(&bufferDescription, sizeof(bufferDescription));

	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.ByteWidth = sizeof(Graphic::Vertex) * Cube->DrawCount;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	result = Device->CreateBuffer(&bufferDescription, NULL, &VertexBuffer);

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
