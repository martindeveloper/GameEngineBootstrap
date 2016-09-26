#include "GameEntity.h"

Core::GameEntity::~GameEntity()
{
	DestroyAllComponents();
}

void Core::GameEntity::SetRenderer(Renderer::IWindowRenderer * renderer)
{
	Renderer = renderer;
}

void Core::GameEntity::AttachComponentInternal(Components::IComponent* component, const char* name)
{
	component->Owner = this;

	bool isExist = IsComponentAttached(name);

	if (isExist)
	{
		// TODO(martin.pernica): Do not throw exception
		throw new std::exception("Component is already registered!");
	}

	auto pair = std::make_pair(name, component);
	Components.insert(pair);
}

void Core::GameEntity::DetachComponent()
{
	throw new std::exception("Not implemented yet!");
}

bool Core::GameEntity::IsComponentAttached(const char * name)
{
	return (Components.count(name) != 0);
}

Components::IComponent* Core::GameEntity::GetComponentInternal(const char* name)
{
	assert(IsComponentAttached(name) == true);

	return Components.at(name);
}

void Core::GameEntity::DestroyAllComponents()
{
	for (ComponentsMap::iterator it = Components.begin(); it != Components.end(); ++it)
	{
		delete it->second;
	}

	Components.clear();
}

Components::RendererComponent* Core::GameEntity::GetRendererComponent()
{
	Components::RendererComponent* rendererComponent = GetComponent<Components::RendererComponent>();

	return rendererComponent;
}
