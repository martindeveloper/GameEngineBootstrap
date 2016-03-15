Texture2D shaderTexture;
SamplerState SampleType;

struct VS_OUTPUT
{
	float4 Position : SV_POSITION0;
	float4 Diffuse : COLOR0;
	float2 UV : TEXCOORD0;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float4 textureColor;

	textureColor = shaderTexture.Sample(SampleType, input.UV) * input.Diffuse;

	return textureColor;
}