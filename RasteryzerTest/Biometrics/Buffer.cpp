#include "Buffer.h"
#include "Structures.h"
#include <algorithm>


void Buffer::setSize(int h, int w)
{
	this->w = w;
	this->h = h;
	this->pixels = new unsigned int * [w];
	for (int i = 0; i < w; i++)
		this->pixels[i] = new unsigned int[h];
	this->depth = new unsigned int* [w];
	for (int i = 0; i < w; i++)
		this->depth[i] = new unsigned int[h];
}

void Buffer::clearColor()
{
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			pixels[i, j] = 0;
}

void Buffer::clearColor(unsigned char c)
{
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			pixels[i][j] = c | c << 8 | c << 16;
}

void Buffer::clearColor(unsigned char r, unsigned char g, unsigned char b)
{
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			pixels[i][j] = b | g << 8 | r << 16;
}

void Buffer::clearDepth()
{
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			depth[i][j] = DEPTH;
}

void Buffer::setBordersAndCons(float4* ver1, float4* ver2, float4* ver3)
{
	minx = std::min(std::min(ver1->x, ver2->x), ver3->x);
	miny = std::min(std::min(ver1->y, ver2->y), ver3->y);
	maxx = std::max(std::max(ver1->x, ver2->x), ver3->x);
	maxy = std::max(std::max(ver1->y, ver2->y), ver3->y);
	dx12 = ver1->x - ver2->x;
	dx23 = ver2->x - ver3->x;
	dx31 = ver3->x - ver1->x;
	dy12 = ver1->y - ver2->y;
	dy23 = ver2->y - ver3->y;
	dy31 = ver3->y - ver1->y;
	if (dy12 < 0 || (dy12 == 0 && dx12 > 0))
		tl1 = true;
	else
		tl1 = false;
	if (dy23 < 0 || (dy23 == 0 && dx23 > 0))
		tl2 = true;
	else
		tl2 = false;
	if (dy31 < 0 || (dy31 == 0 && dx31 > 0))
		tl3 = true;
	else
		tl3 = false;
}

void Buffer::culling()
{
	minx = std::max(minx, 0);
	miny = std::max(miny, 0);
	maxx = std::min(maxx, w - 1);
	maxy = std::min(maxy, h - 1);
}

Buffer::Buffer()
{
}


Buffer::~Buffer()
{
}
