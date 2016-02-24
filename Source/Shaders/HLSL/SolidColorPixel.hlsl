struct VS_OUTPUT
{
	float4 Position : SV_POSITION0;
	float4 Diffuse : COLOR0;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
	return input.Diffuse;
}