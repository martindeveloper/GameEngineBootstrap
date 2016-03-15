#pragma once

#include "../../Common.h"
#include "../../Core/GameEntity.h"

#include "../../Image/Image.h"
#include "../../Image/ImageBMP.h"
#include "../../Math/Math.h"
#include "../../Graphic/Model/ObjModel.h"

namespace Game
{
	namespace Entities
	{
		class CubeEntity : public Core::GameEntity
		{
		public:
			CubeEntity(std::string name) : Core::GameEntity::GameEntity(name) {};
			~CubeEntity();

			void OnLoad();
			void OnUpdate(const double deltaTime);

			std::vector<Graphic::Vertex>* GetVerticies() const;
			Renderer::Material* GetMaterial() const;

			uint32 GetVertexBufferWidth() const;
			uint32 GetVertexBufferStride() const;

		private:
			Math::Vector3<float> Position;
			Graphic::Model::ObjModel* CubeModel;
			Renderer::Material* CurrentMaterial;
		};
	}
}