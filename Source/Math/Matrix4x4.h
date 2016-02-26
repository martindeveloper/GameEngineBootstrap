#pragma once

#include "Vector3.h"
#include "Vector4.h"

#pragma pack(push, 1)
namespace Math
{
	struct Matrix4x4
	{
		Vector4<float> Rows[4];

		Matrix4x4()
		{
			Rows[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
			Rows[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
			Rows[2] = { 0.0f, 0.0f, 1.0f, 0.0f };
			Rows[3] = { 0.0f, 0.0f, 0.0f, 1.0f };
		};

		Matrix4x4(const float* source) : Matrix4x4()
		{
			for (unsigned __int16 i = 0; i < 4; i++)
			{
				unsigned __int16 y = i * 4;

				Rows[i].X = source[y];
				Rows[i].Y = source[y + 1];
				Rows[i].Z = source[y + 2];
				Rows[i].W = source[y + 3];
			}
		};

		Matrix4x4(float scale) : Matrix4x4()
		{
			for (unsigned __int16 i = 0; i < 3; i++)
			{
				Rows[i] = Rows[i] * scale;
			}
		};

		Matrix4x4(Vector3<float> scale) : Matrix4x4()
		{
			for (unsigned __int16 i = 0; i < 3; i++)
			{
				Rows[i] = Rows[i] * scale;
			}
		};

		Matrix4x4 operator*(Matrix4x4 matrix)
		{
			Matrix4x4 result(*this);

			for (unsigned __int16 i = 0; i < 3; i++)
			{
				result.Rows[i] = this->Rows[i] * matrix.Rows[i];
			}

			return result;
		};
	};
};
#pragma pack(pop)