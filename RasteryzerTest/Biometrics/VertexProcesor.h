#pragma once
#include "Structures.h"
class VertexProcesor
{
public:
	float4x4 obj2world;
	float4x4 world2view;
	float4x4 view2proj;
	void setPerspective(float& fovy, float& aspect, float&, float&);
	void setLookat(float3& eye, float3& center, float3 up);
	void multByTranslation(float3& v);
	void multByScale(float3& v);
	void multByRotation(float& a, float3& v);
	void clear();
	VertexProcesor();
	~VertexProcesor();
};

