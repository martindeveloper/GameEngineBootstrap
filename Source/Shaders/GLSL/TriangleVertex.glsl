#version 420

layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec4 vertexColor;

out vec4 fragmentColor;

void main()
{
	gl_Position = vec4(vertexPosition, 1.0);

	fragmentColor = vertexColor;
}