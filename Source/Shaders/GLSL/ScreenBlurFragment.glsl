#version 430
#pragma optimize(off)
#pragma debug(on)

in vec2 frameBufferTextureUV;

uniform sampler2D frameBufferTextureSampler;

out vec4 finalFragmentColor;

const float blurSizeH = 1.0 / 300.0;
const float blurSizeV = 1.0 / 200.0;

void main()
{
	vec4 sum = vec4(0.0);

	for (int x = -4; x <= 4; x++)
	{
		for (int y = -4; y <= 4; y++) 
		{
			sum += texture(frameBufferTextureSampler, vec2(frameBufferTextureUV.x + x * blurSizeH, frameBufferTextureUV.y + y * blurSizeV)) / 81.0;
		}
	}

	finalFragmentColor = sum;
}