#pragma once

#include "../../Math/Matrix4x4.h"

#pragma pack(push, 1)
namespace Graphic
{
	namespace Buffer
	{
		__declspec(align(16))
		struct ConstantBuffer
		{
			Math::Matrix4x4 ModelViewProjectionMatrix;
			unsigned __int32 FrameNumber;
		};
	};
};
#pragma pack(pop)