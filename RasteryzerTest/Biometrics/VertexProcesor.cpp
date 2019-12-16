#include "VertexProcesor.h"

void VertexProcesor::setPerspective(float& fovy, float& aspect, float& near, float& far)
{
	float tmp = fovy * (PI / 360.0f);
	float f = cos(tmp) / sin(tmp);
	view2proj = float4x4(	float4(f / aspect, 0, 0, 0),
							float4(0, f, 0, 0),
							float4(0, 0, (far + near) / (near - far), -1),
							float4(0, 0, (2 * far * near) / (near - far), 0));
}

void VertexProcesor::setLookat(float3& eye, float3& center, float3 up)
{
	float3 f = center - eye;
	f.normalize();
	up.normalize();
	float3 s = f.cross(up);
	float3 u = s.cross(f);
	world2view = float4x4(	float4(s.x, u.x, -f.x, 0),
							float4(s.y, u.y, -f.y, 0),
							float4(s.z, u.z, -f.z, 0),
							float4(-eye.x, -eye.y, -eye.z, 1));
}

void VertexProcesor::multByTranslation(float3& v)
{
	float4x4 m(	float4(1, 0, 0, 0),
				float4(0, 1, 0, 0),
				float4(0, 0, 1, 0),
				float4(v.x, v.y, v.z, 1));
	obj2world = m * obj2world;
}

void VertexProcesor::multByScale(float3& v)
{
	float4x4 m(	float4(v.x, 0, 0, 0),
				float4(0, v.y, 0, 0),
				float4(0, 0, v.z, 0),
				float4(0, 0, 0, 1));
	obj2world = m * obj2world;
}

void VertexProcesor::multByRotation(float& a, float3& v)
{
	float s = sin(a * PI / 180);
	float c = cos(a * PI / 180);
	v.normalize();
	float4x4 m(	float4(v.x * v.x * (1 - c) + c,			v.y * v.x * (1 - c) + v.z * s,	v.x * v.z * (1 - c) - v.y * s,	0),
				float4(v.y * v.x * (1 - c) - v.z * s,	v.y * v.y * (1 - c) + c,		v.y * v.z * (1 - c) + v.x * s,	0),
				float4(v.x * v.z * (1 - c) + v.y * s,	v.y * v.z * (1 - c) - v.x * s,	v.z * v.z * (1 - c) + c,		0),
				float4(0,								0,								0,								1));
	obj2world = m * obj2world;
	obj2worldRotation = m * obj2worldRotation;
}

void VertexProcesor::calculateWVP()
{
	//WVP = (obj2world.combine(world2view).combine(view2proj));
	WVP = obj2world * world2view * view2proj;
}

void VertexProcesor::clear()
{
	obj2world = obj2worldRotation = world2view = view2proj = 
	float4x4(	float4(1, 0, 0, 0),
				float4(0, 1, 0, 0),
				float4(0, 0, 1, 0),
				float4(0, 0, 0, 1));

	WVP = float4x4(	float4(1, 1, 1, 1),
					float4(1, 1, 1, 1),
					float4(1, 1, 1, 1),
					float4(1, 1, 1, 1));
}

VertexProcesor::VertexProcesor()
{
	obj2world = obj2worldRotation = world2view = view2proj =
	float4x4(	float4(1, 0, 0, 0),
				float4(0, 1, 0, 0),
				float4(0, 0, 1, 0),
				float4(0, 0, 0, 1));
	WVP = float4x4(	float4(1, 1, 1, 1),
					float4(1, 1, 1, 1),
					float4(1, 1, 1, 1),
					float4(1, 1, 1, 1));
}

VertexProcesor::~VertexProcesor()
{
}
