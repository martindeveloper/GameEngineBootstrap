#version 430

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec4 vertexColor;
layout(location = 4) uniform mat4 ModelViewProjectionMatrix;

out vec4 fragmentColor;

void main()
{
	gl_Position = ModelViewProjectionMatrix * vec4(vertexPosition, 1.0);

	fragmentColor = vertexColor;
}