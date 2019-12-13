#pragma once
#include <vector>
#include "Structures.h"
#include "VertexProcesor.h"

class Object
{
public:
	std::vector<float3> orginalVertices;
	std::vector<float3> normals;
	std::vector<int> triangleIndexes;
	VertexProcesor verProc;
	void calculateNormals();
	Object();
};

