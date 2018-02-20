#version 430
#pragma optimize(off)
#pragma debug(on)

in vec2 frameBufferTextureUV;

uniform sampler2D frameBufferTextureSampler;

out vec4 finalFragmentColor;

void main()
{
	finalFragmentColor = texture(frameBufferTextureSampler, frameBufferTextureUV);
}