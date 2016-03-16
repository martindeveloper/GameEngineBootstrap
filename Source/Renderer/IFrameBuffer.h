#pragma once

namespace Renderer
{
	class IFrameBuffer
	{
		virtual void Create() = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Draw() = 0;
	};
}