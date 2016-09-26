#include "MeshComponent.h"

const char* Components::MeshComponent::Name = "MeshComponent";

Components::MeshComponent::MeshComponent()
{
}

Components::MeshComponent::~MeshComponent()
{
	delete Mesh;
	delete Texture;
}

void Components::MeshComponent::Initialize(Renderer::IWindowRenderer * renderer)
{
	Renderer = renderer;
}

bool Components::MeshComponent::LoadMesh(const char * AssetName)
{
	FileSystem::File cubeObjSource(AssetName);
	Mesh = new Graphic::Model::ObjModel(&cubeObjSource);

	bool isLoaded = Mesh->Load();

	return isLoaded;
}

bool Components::MeshComponent::LoadTexture(const char * AssetName)
{
	FileSystem::File sourceFile(AssetName);
	Image::ImageBMP* sourceBitmap = new Image::ImageBMP(&sourceFile);

	Texture = (Image::Image*)sourceBitmap;

	Renderer->UploadTexture(Owner, Texture);

	// TODO(martin.pernica): Lie, check if texture is uploaded succesfuly
	return true;
}

std::vector<Graphic::Vertex>* Components::MeshComponent::GetVerticies() const
{
	return &Mesh->Vertices;
}
