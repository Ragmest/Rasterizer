#pragma once
#include "float3.h"
class Buffer
{
public:
	int w, h;
	int minx, miny, maxx, maxy;
	float dx12, dx23, dx31, dy12, dy23, dy31;
	bool tl1, tl2, tl3;
	unsigned int** pixels;
	float** depth;
	void setSize(int h, int w);
	void clearColor();
	void clearColor(unsigned char c);
	void clearColor(unsigned char r, unsigned char g, unsigned char b);
	void clearDepth();
	void setBordersAndCons(float3* ver1, float3* ver2, float3* ver3);
	void culling();
	Buffer();
	~Buffer();
};

