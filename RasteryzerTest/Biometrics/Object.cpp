#include "Object.h"

void Object::calculateNormals()
	{
	float3 n;
	for (int i = 0; i < orginalVertices.size(); ++i)
		normals.push_back(float3());
	for (int i = 0; i < triangleIndexes.size(); i+= 3)
	{
		n = ((orginalVertices.at(triangleIndexes.at(i + 2)) - orginalVertices.at(triangleIndexes.at(i)))
			.cross(orginalVertices.at(triangleIndexes.at(i + 1)) - orginalVertices.at(triangleIndexes.at(i)))).normalizeProd();
		normals.at(triangleIndexes.at(i)) = n + normals.at(triangleIndexes.at(i));
		normals.at(triangleIndexes.at(i + 1)) = n + normals.at(triangleIndexes.at(i + 1));
		normals.at(triangleIndexes.at(i + 2)) = n + normals.at(triangleIndexes.at(i + 2));
	}

	for (int i = 0; i < normals.size(); ++i)
	{
		normals.at(i) = orginalVertices.at(i);
		normals.at(i).normalize();
	}

}

Object::Object()
{
	orginalVertices = std::vector<float3>();
	normals = std::vector<float3>();
	triangleIndexes = std::vector<int>();
}
