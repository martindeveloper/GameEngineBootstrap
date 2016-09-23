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
	Components::RendererComponent* rendererComponent = new Components::RendererComponent();

	AttachComponent(rendererComponent);

	rendererComponent->SetPrimaryMaterial(primaryMaterial);

	// Mesh Component
	Components::MeshComponent* meshComponent = new Components::MeshComponent(Renderer);

	AttachComponent(meshComponent);

	meshComponent->LoadMesh("CubeModel.obj");
	meshComponent->LoadTexture("lenaColor512A.bmp");
}

void CubeEntity::OnUpdate(double deltaTime)
{
	//CurrentMaterial->Transform.Position.X += sin(deltaTime) / 1000;
}