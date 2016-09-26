#include "RendererComponent.h"
#include "../../Core/GameEntity.h"

const char* Components::RendererComponent::Name = "RendererComponent";

Components::RendererComponent::RendererComponent()
{
	Materials.resize(1);
}

Components::RendererComponent::~RendererComponent()
{
	Materials.clear();
}

std::vector<Graphic::Vertex>* Components::RendererComponent::GetVerticies() const
{
	// TODO(martin.pernica): Bad solution, refactor getting mesh comp
	Components::MeshComponent* mesh = Owner->GetComponent<Components::MeshComponent>();

	assert(mesh != nullptr);

	return mesh->GetVerticies();
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

uint32 Components::RendererComponent::GetVertexBufferWidth() const
{
	Components::MeshComponent* mesh = Owner->GetComponent<Components::MeshComponent>();

	assert(mesh != nullptr);

	std::vector<Graphic::Vertex>* verticies = mesh->GetVerticies();

	static const uint32 vertexBufferWidth = sizeof(Graphic::Vertex) * verticies->size();

	return vertexBufferWidth;
}

uint32 Components::RendererComponent::GetVertexBufferStride() const
{
	static const uint32 strideSize = sizeof(Graphic::Vertex);

	return strideSize;
}
