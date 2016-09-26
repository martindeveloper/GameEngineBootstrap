#pragma once

namespace Core
{
	class GameEntity;
}

namespace Components
{
	class IComponent
	{
	public:
		IComponent() {};
		virtual ~IComponent() {};

		Core::GameEntity* Owner = nullptr;
	};
}