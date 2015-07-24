#include "D3D11Renderer.h"

using namespace Renderer;

D3D11Renderer::D3D11Renderer()
{
}

D3D11Renderer::~D3D11Renderer()
{
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
	swapChainDescription.SampleDesc.Count = 1; // multi-samples
	swapChainDescription.Windowed = isWindowed;
	swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH ;
	swapChainDescription.BufferDesc.Width = windowRectangle.right;
	swapChainDescription.BufferDesc.Height = windowRectangle.bottom;

	UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif 

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

	PrepareBuffers();

	HRESULT result = S_OK;

	// Triangle primitive
	Graphic::Primitive::TrianglePrimitive triangle;

	D3D11_MAPPED_SUBRESOURCE mappedSubResource;

	result = DeviceContext->Map(VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSubResource);
	memcpy(mappedSubResource.pData, &triangle, sizeof(triangle));
	DeviceContext->Unmap(VertexBuffer, NULL);

	assert(!(FAILED(result)));

	// Create and set viewport
	D3D11_VIEWPORT viewport = {0, 0, 1024, 768, 0.0f, 1.0f };
	DeviceContext->RSSetViewports(1, &viewport);

	CreateShaders();

	D3D11_RASTERIZER_DESC rasterizerDescription;
	ZeroMemory(&rasterizerDescription, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDescription.FillMode = D3D11_FILL_SOLID;
	rasterizerDescription.CullMode = D3D11_CULL_NONE;

	Device->CreateRasterizerState(&rasterizerDescription, &RasterizerState);
}

void D3D11Renderer::ClearWindow(double deltaTime)
{
	const FLOAT color[4] = { sin(deltaTime) * 0.5f + 0.5f, cos(deltaTime) * 0.5f + 0.5f, 0.0f, 1.0f };
	DeviceContext->ClearRenderTargetView(BackBuffer, color);
}

void D3D11Renderer::Update(double deltaTime)
{

}

void D3D11Renderer::Render(double deltaTime)
{
	HRESULT result = S_OK;

	UINT stride = sizeof(Graphic::Vertex);
	UINT offset = 0;

	DeviceContext->VSSetShader(RotationVertexShader, nullptr, 0);
	DeviceContext->PSSetShader(SolidColorPixelShader, nullptr, 0);

	DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Modify constant values
	{
		static unsigned __int32 frameCounter = 0;

		D3D11_MAPPED_SUBRESOURCE constantBufferMappedResource;
		result = DeviceContext->Map(UniformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantBufferMappedResource);
		Graphic::Buffer::ConstantBuffer* mappedConstantBuffer = (Graphic::Buffer::ConstantBuffer*)constantBufferMappedResource.pData;

		Math::Matrix4x4 projectionMatrix(Math::Vector3<float>(.8f, .3f, 1.0f));
		Math::Matrix4x4 viewMatrix;
		Math::Matrix4x4 modelMatrix;

		const Math::Matrix4x4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

		mappedConstantBuffer->ModelViewProjectionMatrix = mvpMatrix;
		mappedConstantBuffer->FrameNumber = frameCounter;

		DeviceContext->Unmap(UniformBuffer, 0);

		frameCounter++;
	}

	DeviceContext->RSSetState(RasterizerState);

	// Draw
	DeviceContext->Draw(3, 0);

	// Swap
	result = SwapChain->Present(0, 0);

	assert(!(FAILED(result)));
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

	std::vector<char> vertexShaderBytes = vertexShaderFile.GetBinaryContent();
	size_t vertexShaderBytesSize = vertexShaderBytes.size() * sizeof(char);
	const char* vertexShaderBytesPointer = &vertexShaderBytes[0];

	result = Device->CreateVertexShader(vertexShaderBytesPointer, vertexShaderBytesSize, nullptr, &RotationVertexShader);

	// Pixel shader
	FileSystem::File pixelShaderFile("SolidColorPixel.cso");

	std::vector<char> pixelShaderBytes = pixelShaderFile.GetBinaryContent();
	size_t pixelShaderBytesSize = pixelShaderBytes.size() * sizeof(char);
	const char* pixelShaderBytesPointer = &pixelShaderBytes[0];

	result = Device->CreatePixelShader(pixelShaderBytesPointer, pixelShaderBytesSize, nullptr, &SolidColorPixelShader);

	// Input layout
	const D3D11_INPUT_ELEMENT_DESC inputElementDescription[] =
	{
		{ "VERTEX_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "VERTEX_COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
	bufferDescription.ByteWidth = sizeof(Graphic::Vertex) * 3;
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
