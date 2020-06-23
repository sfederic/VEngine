cbuffer cbPerObject
{
	row_major float4x4 model;
	row_major float4x4 view;
	row_major float4x4 proj;
	row_major float4x4 mvp;
};

struct VS_IN
{
	float3 pos: POSITION;
};

struct VS_OUT
{
	float4 pos: SV_POSITION;
};

VS_OUT VSMain(VS_IN i)
{
	VS_OUT o;
	o.pos = mul(float4(i.pos, 1.0f), mvp);

	return o;
}

float4 PSMain(VS_OUT i) : SV_Target
{
	return float4(1.0f, 1.0f, 0.f, 1.f);
}