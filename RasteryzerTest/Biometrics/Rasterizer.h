#pragma once
#include "Buffer.h"
#include <vector>
#include <opencv2/core/core.hpp>

class Rasterizer
{
private: 
	Buffer buffer;
	std::vector<float3> triangles;
	std::vector<int> triangleIndexes;
	void Triangulate(float3* ver1W, float3* ver2W, float3* ver3W);
public:
	void Rasterize();
	void getImage(cv::Mat* image);

	Rasterizer();
};

