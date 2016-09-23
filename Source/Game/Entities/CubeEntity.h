#pragma once

#include "../../Common.h"
#include "../../Core/GameEntity.h"

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
			virtual ~CubeEntity();

			bool IsStatic() const override { return true; };
			void OnLoad();
			void OnUpdate(const double deltaTime);

			// TODO(martin.pernica): Remove this prop, only for testing
			Math::Vector3<float> StartingPosition;

		private:
			Math::Vector3<float> Position;
		};
	}
}