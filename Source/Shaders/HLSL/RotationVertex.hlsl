cbuffer ConstantBuffer : register(b0)
{
	float4x4 ModelViewProjectionMatrix;
	uint FrameNumber;
};

cbuffer TransformBuffer : register(b1)
{
	float3 Position;
	float3 Scale;
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

	float radians = ((FrameNumber / 100) * 3.14159f) / 180.0f;

	float3 basePosition = input.Position * Scale;

	float x = (basePosition.x * cos(radians)) + (basePosition.y * sin(radians));
	float y = (basePosition.x * sin(radians)) - (basePosition.y * cos(radians));

	float3 vertexPosition = float3(x, y , basePosition.z);

	vertexStageOutput.Position = mul(ModelViewProjectionMatrix, float4(vertexPosition, 1.0f));
	vertexStageOutput.Diffuse = input.Diffuse;
	vertexStageOutput.UV = input.UV;

	return vertexStageOutput;
}