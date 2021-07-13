module;
#include <DirectXMath.h>
export module Math;

export struct float4 : DirectX::XMFLOAT4
{
	float4() { x = 0.f, y = 0.f, z = 0.f, w = 0.f; }
	float4(float f) { x = f; y = f; z = f; w = f; }
	float4(float x_, float y_, float z_, float w_) { x = x_; y = y_; z = z_; w = w_; }
	float4 operator+(float4& rhs) { return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w }; }
	float4 operator-(float4& rhs) { return { x - rhs.x, y - rhs.y, z - rhs.z, w + rhs.w }; }
	float4 operator*(float4& rhs) { return { x * rhs.x, y * rhs.y, z * rhs.z, w + rhs.w }; }
	float4 operator/(float4& rhs) { return { x / rhs.x, y / rhs.y, z / rhs.z, w + rhs.w }; }
};

export struct float3 : DirectX::XMFLOAT3
{
	float3() { x = 0.f, y = 0.f, z = 0.f; }
	float3(float f) { x = f; y = f; z = f; }
	float3(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }
	float3 operator+(float3& rhs) { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
	float3 operator-(float3& rhs) { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
	float3 operator*(float3& rhs) { return { x * rhs.x, y * rhs.y, z * rhs.z }; }
	float3 operator/(float3& rhs) { return { x / rhs.x, y / rhs.y, z / rhs.z }; }
};

export struct float2 : DirectX::XMFLOAT2
{
	float2() { x = 0.f, y = 0.f; }
	float2(float f) { x = f; y = f; }
	float2(float x_, float y_) { x = x_; y = y_; }
	float2 operator+(float2& rhs) { return { x + rhs.x, y + rhs.y }; }
	float2 operator-(float2& rhs) { return { x - rhs.x, y - rhs.y }; }
	float2 operator*(float2& rhs) { return { x * rhs.x, y * rhs.y }; }
	float2 operator/(float2& rhs) { return { x / rhs.x, y / rhs.y }; }
};

export struct int4 : DirectX::XMINT4
{
	int4() { x = 0, y = 0; z = 0; w = 0; }
	int4(int i) { x = i, y = i; z = i; w = i; }
	int4(int x_, int y_, int z_, int w_) { x = x_, y = y_; z = z_; w = w_; }
	int4 operator+(int4& rhs) { return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w }; }
	int4 operator-(int4& rhs) { return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w }; }
	int4 operator*(int4& rhs) { return { x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w }; }
	int4 operator/(int4& rhs) { return { x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w }; }
};

export struct int3 : DirectX::XMINT3
{
	int3() { x = 0, y = 0; z = 0; }
	int3(int i) { x = i, y = i; z = i; }
	int3(int x_, int y_, int z_) { x = x_, y = y_; z = z_; }
	int3 operator+(int3& rhs) { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
	int3 operator-(int3& rhs) { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
	int3 operator*(int3& rhs) { return { x * rhs.x, y * rhs.y, z * rhs.z }; }
	int3 operator/(int3& rhs) { return { x / rhs.x, y / rhs.y, z / rhs.z }; }
};

export struct int2 : DirectX::XMINT2
{
	int2() { x = 0, y = 0; }
	int2(int i) { x = i, y = i; }
	int2(int x_, int y_) { x = x_, y = y_; }
	int2 operator+(int2& rhs) { return { x + rhs.x, y + rhs.y }; }
	int2 operator-(int2& rhs) { return { x - rhs.x, y - rhs.y }; }
	int2 operator*(int2& rhs) { return { x * rhs.x, y * rhs.y }; }
	int2 operator/(int2& rhs) { return { x / rhs.x, y / rhs.y }; }
};
