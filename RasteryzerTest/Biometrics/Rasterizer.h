#pragma once
#include "Buffer.h"
#include <vector>
#include <opencv2/core/core.hpp>
#include "VertexProcesor.h"
#include "Object.h"
#include "DirectionalLight.h"
#include "float4.h"

class Rasterizer
{
private: 
	Buffer buffer;
	std::vector<float4> vertices;
	std::vector<float4> normals;
	std::vector<int> triangleIndexes;
	std::vector<int> objectIndexes;
	DirectionalLight* light;
	float3* eye;
	void triangulate(float3* ver1W, float3* ver2W, float3* ver3W, float3* col1, float3* col2, float3* col3);
	void applayMatrices();
	void setUpVerticlesOfObjects();
public:
	std::vector<Object> objects;
	void rasterize();
	void getImage(cv::Mat* image);

	Rasterizer(std::vector<Object>* objects, DirectionalLight* light, float3* eye);
};

