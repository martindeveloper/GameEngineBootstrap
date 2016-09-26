#include "MeshComponent.h"

Components::MeshComponent::MeshComponent()
{
}

Components::MeshComponent::~MeshComponent()
{
	delete Mesh;
	delete DiffuseTexture;
}

void Components::MeshComponent::Initialize(Renderer::IWindowRenderer * renderer)
{
	Renderer = renderer;
}

bool Components::MeshComponent::SetMesh(const char* AssetName)
{
	FileSystem::File cubeObjSource(AssetName);
	Mesh = new Graphic::Model::ObjModel(&cubeObjSource);

	bool isLoaded = Mesh->Load();

	return isLoaded;
}

bool Components::MeshComponent::SetTexture(const char* AssetName, TextureSlot slot)
{
	// TODO(martin.pernica): Texture slot arg is unused
	FileSystem::File* sourceFile = new FileSystem::File(AssetName);

	assert(sourceFile->IsExists() == true);

	Image::ImageBMP* sourceBitmap = new Image::ImageBMP(sourceFile);

	DiffuseTexture = (Image::Image*)sourceBitmap;

	// TODO(martin.pernica): Lie, check if texture is uploaded succesfuly
	return true;
}

Image::Image * Components::MeshComponent::GetTexture(TextureSlot slot)
{
	switch (slot)
	{
	case TEXTURE_DIFFUSE_SLOT:
		return DiffuseTexture;
		break;

	default:
		return DiffuseTexture;
		break;
	}
}

std::vector<Graphic::Vertex>* Components::MeshComponent::GetVerticies() const
{
	return &Mesh->Vertices;
}

uint32 Components::MeshComponent::GetVertexBufferWidth() const
{
	static const uint32 vertexBufferWidth = sizeof(Graphic::Vertex) * Mesh->Vertices.size();

	return vertexBufferWidth;
}

uint32 Components::MeshComponent::GetVertexBufferStride() const
{
	static const uint32 strideSize = sizeof(Graphic::Vertex);

	return strideSize;
}