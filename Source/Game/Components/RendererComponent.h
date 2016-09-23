#pragma once

#include "../../Common.h"
#include "IComponent.h"
#include "../../Renderer/Material.h"
#include "MeshComponent.h"

namespace Core
{
	class GameEntity;
}

namespace Components
{
	class RendererComponent : public IComponent
	{
	public:
		static const char* Name;
		const char* GetName() const override { return Name; };

		RendererComponent();
		virtual ~RendererComponent();

		std::vector<Renderer::Material*> Materials;

		std::vector<Graphic::Vertex>* GetVerticies() const;
		Renderer::Material* GetPrimaryMaterial() const;
		void SetPrimaryMaterial(Renderer::Material* material);

		uint32 GetVertexBufferWidth() const;
		uint32 GetVertexBufferStride() const;
	};
}