#include "D3D11FrameBuffer.h"

using namespace Renderer;

D3D11FrameBuffer::D3D11FrameBuffer(Renderer::D3D11Renderer* renderer)
{
	Renderer = renderer;
}

D3D11FrameBuffer::~D3D11FrameBuffer()
{
	BackBuffer->Release();
	//FrontBuffer->Release();
}

void D3D11FrameBuffer::Create()
{
	CreateBuffer(&BackBuffer);
	//CreateBuffer(&FrontBuffer);
}

void D3D11FrameBuffer::Bind()
{
	Renderer->DeviceContext->OMSetRenderTargets(1, &BackBuffer, Renderer->DepthStencilView);
}

void D3D11FrameBuffer::Unbind()
{
	//Renderer->DeviceContext->OMSetRenderTargets(1, &FrontBuffer, NULL);
}

void D3D11FrameBuffer::Draw()
{

}

void D3D11FrameBuffer::CreateBuffer(ID3D11RenderTargetView** buffer)
{
	HRESULT result = S_OK;

	ID3D11Texture2D *pBufferTexture;

	result = Renderer->SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBufferTexture);
	result = Renderer->Device->CreateRenderTargetView(pBufferTexture, NULL, buffer);

	pBufferTexture->Release();

	assert(!(FAILED(result)));
}