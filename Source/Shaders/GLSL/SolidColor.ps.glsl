#version 430
#pragma optimize(off)
#pragma debug(on)

in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 finalFragmentColor;

uniform sampler2D mainTextureSampler;

void main()
{
	finalFragmentColor = texture(mainTextureSampler, fragmentUV) * fragmentColor;
}