#version 430

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec4 vertexColor;
layout(location = 4) uniform mat4 ModelViewProjectionMatrix;
layout(location = 5) uniform float frameNumber;

out vec4 fragmentColor;

void main()
{
	float radians = (frameNumber * 3.14159f) / 180.0f;

	float x = (vertexPosition.x * cos(radians)) + (vertexPosition.y * sin(radians));
	float y = (vertexPosition.x * sin(radians)) - (vertexPosition.y * cos(radians));

	gl_Position = ModelViewProjectionMatrix * vec4(x, y, vertexPosition.z, 1.0);

	fragmentColor = vertexColor;
}