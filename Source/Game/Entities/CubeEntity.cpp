#include "CubeEntity.h"

using namespace Game::Entities;

CubeEntity::~CubeEntity()
{
}

void CubeEntity::OnLoad()
{
	// Material
	// NOTE(martin.pernica): Currently every material is deleted by renderer.
	//                       Maybe it would be better delete it in entity itself
	Renderer::Material* primaryMaterial = Renderer->CreateMaterial();
	primaryMaterial->VertexShader = "RotationVertex";
	primaryMaterial->PixelShader = "SolidColorFragment";
	primaryMaterial->Transform = { StartingPosition,{ 0.5f, 0.5f, 0.5f } };

	// Renderer Component
	Components::RendererComponent* rendererComponent = AttachComponent<Components::RendererComponent>();
	rendererComponent->SetPrimaryMaterial(primaryMaterial);

	// Mesh Component
	Components::MeshComponent* meshComponent = AttachComponent<Components::MeshComponent>();

	bool isMeshLoaded = meshComponent->SetMesh("Resources/CubeModel.obj");

	assert(isMeshLoaded == true);

	meshComponent->SetTexture("Resources/lenaColor512A.bmp", Components::TextureSlot::TEXTURE_DIFFUSE_SLOT);
}

void CubeEntity::OnUpdate(double deltaTime)
{
	GetComponent<Components::RendererComponent>()->GetPrimaryMaterial()->Transform.Position.X += static_cast<float>(sin(deltaTime) / 1000.0);
}