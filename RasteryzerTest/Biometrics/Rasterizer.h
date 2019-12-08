#pragma once
#include "Buffer.h"
#include <vector>
#include <opencv2/core/core.hpp>
#include "VertexProcesor.h"

class Rasterizer
{
private: 
	Buffer buffer;
	std::vector<float4> vertices;
	std::vector<float4> orginalVertices;
	std::vector<int> triangleIndexes;
	void triangulate(float4* ver1W, float4* ver2W, float4* ver3W);
	void applayMatrices();
public:
	VertexProcesor vertProc;
	void rasterize();
	void getImage(cv::Mat* image);

	Rasterizer();
};

