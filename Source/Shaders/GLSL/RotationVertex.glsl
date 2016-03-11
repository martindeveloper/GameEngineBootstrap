#version 430
#pragma optimize(off)
#pragma debug(on)

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) uniform mat4 ModelViewProjectionMatrix;
layout(location = 4) uniform float frameNumber;

out vec4 fragmentColor;
out vec2 fragmentUV;

void main()
{
	float radians = (frameNumber * 3.14159f) / 180.0f;

	float x = (vertexPosition.x * cos(radians)) + (vertexPosition.y * sin(radians));
	float y = (vertexPosition.x * sin(radians)) - (vertexPosition.y * cos(radians));

	gl_Position = ModelViewProjectionMatrix * vec4(x, y, vertexPosition.z, 1.0);

	fragmentColor = vertexColor;
	fragmentUV = vertexUV;
}