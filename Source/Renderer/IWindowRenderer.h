#pragma once

#include "../Common.h"
#include "../Image/Image.h"
#include "../Core/GameEntity.h"

namespace Renderer {
	struct RendererParameters
	{
		int Width;
		int Height;
	};

	class IWindowRenderer
	{
	public:
		virtual ~IWindowRenderer() {};

		RendererParameters Parameters;

		virtual void BeforeStart(HDC WindowDeviceContext, const bool isWindowed) = 0;
		virtual void ClearWindow(const double deltaTime) = 0;
		virtual void Update(const double deltaTime) = 0;
		virtual void Render(const double deltaTime) = 0;

		virtual Renderer::Material* CreateMaterial() = 0;
		virtual void UploadTexture(Core::GameEntity* entity, Image::Image* image) = 0;

	protected:
		// Entities
		std::vector<Core::GameEntity*> GameEntitites;

		virtual void CreateShaderForEntity(Core::GameEntity* entity) = 0;
		virtual void PrepareBuffers() = 0;
		//virtual void CreateConstantBuffer() = 0;
	};
}