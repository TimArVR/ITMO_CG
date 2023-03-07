struct VSOut
{
	float4 color : Color;
	float4 pos : SV_Position;
};

cbuffer CBuf : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
};

VSOut main(float4 pos : Position, float4 color : Color)
{
	VSOut vso;
	vso.pos = mul(pos, World);
	vso.pos = mul(vso.pos, View);
	vso.pos = mul(vso.pos, Projection);
	vso.color = color;
	return vso;
}