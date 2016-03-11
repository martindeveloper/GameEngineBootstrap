cbuffer ConstantBuffer : register(b0)
{
	float4x4 ModelViewProjectionMatrix;
	uint FrameNumber;
};

struct VS_INPUT
{
	float3 Position : VERTEX_POSITION0;
	float4 Diffuse : VERTEX_COLOR0;
	float2 UV : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION0;
	float4 Diffuse : COLOR0;
	float2 UV : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT vertexStageOutput;

	float radians = ((FrameNumber/100) * 3.14159f) / 180.0f;

	float x = (input.Position.x * cos(radians)) + (input.Position.y * sin(radians));
	float y = (input.Position.x * sin(radians)) - (input.Position.y * cos(radians));

	vertexStageOutput.Position = mul(ModelViewProjectionMatrix, float4(x, y, input.Position.z, 1.0f));
	vertexStageOutput.Diffuse = input.Diffuse;
	vertexStageOutput.UV = input.UV;

	return vertexStageOutput;
}