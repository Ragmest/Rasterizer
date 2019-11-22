#pragma once
#include <iostream>
#define WIDTH 1024	
#define HEIGHT 1024
#define DEPTH 1024

struct float3
{
	float x, y, z;
	float3() : x(0), y(0), z(0) {}
	float3(float x, float y, float z) : x(x), y(y), z(z) {}

	float lenght()
	{
		return sqrt(x * x + y * y + z * z);
	}

	void normalize()
	{
		float scale = 1 / this->lenght();
		x = x * scale;
		y = y * scale;
		z = z * scale;
	}

	float3 normalizeProd()
	{
		float scale = 1 / this->lenght();
		return float3(x*scale, y*scale, z*scale);
	}

	float dot(const float3& other)
	{
		return x * other.x + y * other.y + z * other.z;
	}

	float3 cross(const float3& other)
	{
		return float3(y*other.z - z * other.y,
			z*other.x - x * other.z,
			x*other.y - y * other.x);
	}

	void reverse()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	float3 reverseProd()
	{
		return float3(-x, -y, -z);
	}

	float3 operator*(const float& other)
	{
		return float3(x*other, y*other, z*other);
	}

	float3 operator+(const float& other)
	{
		return float3(x + other, y + other, z + other);
	}

	friend std::ostream & operator << (std::ostream &out, const float3 &a)
	{
		out << a.x << " " << a.y << " " << a.z << std::endl;
		return out;
	}
};