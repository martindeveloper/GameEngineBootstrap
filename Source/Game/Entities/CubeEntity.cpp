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

	// NOTE(martin.pernica): Renderer is needed because mesh component will upload textures and others. Maybe inverse the dependency and let render itself upload texture afterwards
	meshComponent->Initialize(Renderer);

	bool isMeshLoaded = meshComponent->LoadMesh("CubeModel.obj");

	assert(isMeshLoaded == true);

	meshComponent->LoadTexture("lenaColor512A.bmp");
}

void CubeEntity::OnUpdate(double deltaTime)
{
	GetRendererComponent()->GetPrimaryMaterial()->Transform.Position.X += sin(deltaTime) / 1000;
}