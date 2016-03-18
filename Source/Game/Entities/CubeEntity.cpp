#include "CubeEntity.h"

using namespace Game::Entities;

CubeEntity::~CubeEntity()
{
	delete CubeModel;
}

void CubeEntity::OnLoad()
{
	// Model
	FileSystem::File cubeObjSource("CubeModel.obj");
	CubeModel = new Graphic::Model::ObjModel(&cubeObjSource);
	CubeModel->Load();

	// Material

	// NOTE(martin.pernica): Currently every material is deleted by renderer.
	//                       Maybe it would be better delete it in entity itself
	CurrentMaterial = Renderer->CreateMaterial();
	CurrentMaterial->VertexShader = "RotationVertex";
	CurrentMaterial->PixelShader = "SolidColorFragment";
	CurrentMaterial->Transform = { StartingPosition, { 0.5f, 0.5f, 0.5f } };

	// Texture
	FileSystem::File sourceFile("lenaColor512A.bmp");
	Image::ImageBMP sourceBitmap(&sourceFile);

	Renderer->UploadTexture(this, (Image::Image*)&sourceBitmap);
}

void CubeEntity::OnUpdate(double deltaTime)
{
	CurrentMaterial->Transform.Position.X += sin(deltaTime) / 1000;
}

std::vector<Graphic::Vertex>* CubeEntity::GetVerticies() const
{
	return &CubeModel->Vertices;
}

Renderer::Material* CubeEntity::GetMaterial() const
{
	return CurrentMaterial;
}

uint32 CubeEntity::GetVertexBufferWidth() const
{
	static const uint32 vertexBufferWidth = sizeof(Graphic::Vertex) * CubeModel->Vertices.size();

	return vertexBufferWidth;
}

uint32 CubeEntity::GetVertexBufferStride() const
{
	static const uint32 strideSize = sizeof(Graphic::Vertex);

	return strideSize;
}
