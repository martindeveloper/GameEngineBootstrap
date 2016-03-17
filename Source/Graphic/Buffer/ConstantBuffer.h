#pragma once

#include "../../Math/Matrix4x4.h"

namespace Graphic
{
	namespace Buffer
	{
		struct alignas(16) ConstantBuffer
		{
			alignas(16) Math::Matrix4x4 ModelViewProjectionMatrix;
			alignas(16) uint32 FrameNumber;
		};
	};
};
