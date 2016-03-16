#pragma once

#include "../../Math/Vector3.h"

namespace Graphic
{
	namespace Buffer
	{
		struct alignas(16) TransformBuffer
		{
			Math::Vector3<float> Position;
			Math::Vector3<float> Scale;
		};
	};
};
