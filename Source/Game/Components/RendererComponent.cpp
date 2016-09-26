#include "RendererComponent.h"
#include "../../Core/GameEntity.h"

Components::RendererComponent::RendererComponent()
{
	Materials.resize(1);
}

Components::RendererComponent::~RendererComponent()
{
	Materials.clear();
}

Renderer::Material* Components::RendererComponent::GetPrimaryMaterial() const
{
	Renderer::Material* material = Materials.at(0);

	assert(material != nullptr);

	return material;
}

void Components::RendererComponent::SetPrimaryMaterial(Renderer::Material* material)
{
	assert(Materials.size() > 0);

	// TODO(martin.pernica): Check if zero index is not already used
	Materials[0] = material;
}
