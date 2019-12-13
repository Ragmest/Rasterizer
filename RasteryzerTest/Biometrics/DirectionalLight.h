#pragma once
#include "Structures.h"
class DirectionalLight
{
public:
	float4 position;
	float3 ambient;
	float3 diffuse;
	float3 specular;
	float shininess;
	DirectionalLight();
	DirectionalLight(float4 position, float3 ambient, float3 diffuse, float3 specular, float shininess) : position(position), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess){}
};

