#pragma once

#include "../../Common.h"
#include "IComponent.h"
#include "../../Image/Image.h"
#include "../../Image/ImageBMP.h"
#include "../../Graphic/Model/ObjModel.h"
#include "../../Graphic/Vertex.h"

namespace Renderer
{
	class IWindowRenderer;
	class Material;
}

namespace Components
{
	enum TextureSlot
	{
		TEXTURE_DIFFUSE_SLOT
	};

	class MeshComponent : public IComponent
	{
	public:
		MeshComponent();
		virtual ~MeshComponent();

		bool SetMesh(const char* AssetName);

		bool SetTexture(const char* AssetName, TextureSlot slot);
		Image::Image* GetTexture(TextureSlot slot);

		std::vector<Graphic::Vertex>* GetVerticies() const;
		uint32 GetVertexBufferWidth() const;
		uint32 GetVertexBufferStride() const;

	private:
		Renderer::IWindowRenderer* Renderer;
		Image::Image* DiffuseTexture;
		Graphic::Model::ObjModel* Mesh;
	};
}