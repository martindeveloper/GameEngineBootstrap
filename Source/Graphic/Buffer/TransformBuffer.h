#pragma once

#include "../../Math/Vector3.h"

namespace Graphic
{
	namespace Buffer
	{
		struct alignas(16) TransformBuffer
		{
			alignas(16) Math::Vector3<float> Position;
			alignas(16) Math::Vector3<float> Scale;
		};
	};
};
