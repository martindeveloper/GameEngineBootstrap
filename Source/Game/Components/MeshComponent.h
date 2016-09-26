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
	class MeshComponent : public IComponent
	{
	public:
		static const char* Name;
		const char* GetName() const override { return Name; };

		MeshComponent();
		virtual ~MeshComponent();

		void Initialize(Renderer::IWindowRenderer* renderer);

		bool LoadMesh(const char* AssetName);
		bool LoadTexture(const char* AssetName);

		std::vector<Graphic::Vertex>* GetVerticies() const;

	private:
		Renderer::IWindowRenderer* Renderer;
		Image::Image* Texture;
		Graphic::Model::ObjModel* Mesh;
	};
}