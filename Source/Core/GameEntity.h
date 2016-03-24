#pragma once

#include "../Common.h"
#include "../Graphic/Vertex.h"

namespace Renderer
{
	class IWindowRenderer;
	class Material;
}

namespace Core
{
	class GameEntity
	{
	public:
		const std::string Name;

		GameEntity(std::string name) : Name(name) {};
		virtual ~GameEntity() {};

		virtual bool IsStatic() const { return true; };
		virtual void OnLoad() {};
		virtual void OnUpdate(const double deltaTime) {};

		virtual std::vector<Graphic::Vertex>* GetVerticies() const { return nullptr; };
		virtual Renderer::Material* GetMaterial() const { return nullptr; };

		virtual uint32 GetVertexBufferWidth() const { return 0; }
		virtual uint32 GetVertexBufferStride() const { return 0; };

		void SetRenderer(Renderer::IWindowRenderer* renderer)
		{
			Renderer = renderer;
		};

	protected:
		Renderer::IWindowRenderer* Renderer;
	};
}
