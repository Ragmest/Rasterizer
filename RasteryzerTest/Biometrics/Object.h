#pragma once
#include <vector>
#include "VertexProcesor.h"

class Object
{
public:
	std::vector<float3> orginalVertices;
	std::vector<float3> normals;
	std::vector<int> triangleIndexes;
	float3 *col;
	VertexProcesor verProc;
	void calculateNormals();
	Object(float3* col);
};

