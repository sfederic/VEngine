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
	o.pos = float4(i.pos, 1.0f);

	return o;
}

float4 PSMain(VS_OUT i) : SV_Target
{
	return float4(1.0f, 1.0f, 0.f, 1.f);
}