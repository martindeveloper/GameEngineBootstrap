#version 430
#pragma optimize(off)
#pragma debug(on)

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 vertexUV;

out vec2 frameBufferTextureUV;

void main()
{
	frameBufferTextureUV = vertexUV;
	gl_Position = vec4(vertexPosition, 0.0, 1.0);
}