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

		void DetachComponent();

		template<typename T>
		bool IsComponentAttached()
		{
			const char* name = typeid(T).name();

			return IsComponentAttachedInternal(name);
		}

		template<typename T>
		T* AttachComponent()
		{
			T* component = new T();
			const char* name = typeid(T).name();

			AttachComponentInternal(component, name);

			return component;
		}

		template<typename T>
		T* GetComponent()
		{
			const char* name = typeid(T).name();

			Components::IComponent* component = GetComponentInternal(name);
			T* typedComponent = static_cast<T*>(component);

			assert(typedComponent != nullptr);

			return typedComponent;
		}

		void DestroyAllComponents();

	protected:
		Renderer::IWindowRenderer* Renderer;
		ComponentsMap Components;

	private:
		void AttachComponentInternal(Components::IComponent* component, const char* name);
		Components::IComponent* GetComponentInternal(const char* name);
		bool IsComponentAttachedInternal(const char* name);
	};
}
