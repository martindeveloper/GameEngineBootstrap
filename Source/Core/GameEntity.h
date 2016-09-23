#pragma once

#include "../Common.h"
#include "../Graphic/Vertex.h"
#include "../Game/Components/IComponent.h"
#include "../Game/Components/RendererComponent.h"

namespace Renderer
{
	class IWindowRenderer;
	class Material;
}

typedef std::map<const char*, Components::IComponent*> ComponentsMap;

namespace Core
{
	class GameEntity
	{
	public:
		const std::string Name;

		GameEntity(std::string name) : Name(name) {};

		virtual ~GameEntity();

		virtual bool IsStatic() const { return true; };
		virtual void OnLoad() {};
		virtual void OnUpdate(const double deltaTime) {};

		void SetRenderer(Renderer::IWindowRenderer* renderer);

		void AttachComponent(Components::IComponent* component);
		void DetachComponent();
		bool IsComponentAttached(const char* name);
		Components::IComponent* GetComponent(const char* name);
		void DestroyAllComponents();

		Components::RendererComponent* GetRendererComponent();

	protected:
		Renderer::IWindowRenderer* Renderer;
		ComponentsMap Components;
	};
}
