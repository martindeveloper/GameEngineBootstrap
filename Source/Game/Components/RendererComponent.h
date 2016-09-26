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
		RendererComponent();
		virtual ~RendererComponent();

		std::vector<Renderer::Material*> Materials;

		Renderer::Material* GetPrimaryMaterial() const;
		void SetPrimaryMaterial(Renderer::Material* material);
	};
}