#pragma once

#include "../../Math/Matrix4x4.h"

namespace Graphic
{
	namespace Buffer
	{
		struct alignas(16) ConstantBuffer
		{
			Math::Matrix4x4 ModelViewProjectionMatrix;
			uint32 FrameNumber;
		};
	};
};
