#pragma once
#include <iostream>
#define WIDTH 512	
#define HEIGHT 512
#define DEPTH 512
#define PI 3.14159265359

const float epsilon = 0.0001f;

struct float3
{
	float x, y, z;
	float3() : x(0), y(0), z(0) {}
	float3(float x, float y, float z) : x(x), y(y), z(z) {}

	void ZeroOne()
	{
		if (x > 1.f)
			x = 1;
		if (y > 1.f)
			y = 1;
		if (z > 1.f)
			z = 1;
		if (x < 0.f)
			x = 0;
		if (y < 0.f)
			y = 0;
		if (z < 0.f)
			z = 0;
	}

	float lenght()
	{
		return sqrt(x * x + y * y + z * z);
	}

	void normalize()
	{
		if (this->lenght() > epsilon)
		{
			float scale = 1 / this->lenght();
			x = x * scale;
			y = y * scale;
			z = z * scale;
		}
		else
			x = y = z = 0;
	}

	float3 normalizeProd()
	{
		if (this->lenght() > epsilon)
		{
			float scale = 1 / this->lenght();
			return float3(x * scale, y * scale, z * scale);
		}
		else
			return float3();
	}

	float dot(const float3& other)
	{
		return (x * other.x) + (y * other.y) + (z * other.z);
	}

	float3 cross(const float3& other)
	{
		return float3(	y*other.z - z * other.y,
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

	float3 reflection(float3 normal)
	{
		//return *this - (normal * ( 2 * this->dot(normal)));
		//return (normal * (2 * this->dot(normal))) - *this;
		return *this - (normal * 2 * (this->dot(normal)/normal.dot(normal)));
		//normal of course must be normalized
	}

	float3 operator+(const float& other)
	{
		return float3(x + other, y + other, z + other);
	}

	float3 operator-(const float& other)
	{
		return float3(x - other, y - other, z - other);
	}

	float3 operator+(const float3& other)
	{
		return float3(x + other.x, y + other.y, z + other.z);
	}

	float3 operator-(const float3& other)
	{
		return float3(x - other.x, y - other.y, z - other.z);
	}

	float3 operator*(const float& other)
	{
		return float3(x * other, y * other, z * other);
	}

	float3 operator/(const float& other)
	{
		return float3(x / other, y / other, z / other);
	}

	float3 operator-()
	{
		return float3(-x, -y, -z);
	}

	friend std::ostream & operator << (std::ostream &out, const float3 &a)
	{
		out << a.x << " " << a.y << " " << a.z << std::endl;
		return out;
	}
};

struct float4
{
	float x, y, z, w;
	float4() : x(0), y(0), z(0), w(1) {}
	float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	float4(float x, float y, float z) : x(x), y(y), z(z), w(1) {}
	float4(float3 ver): x(ver.x), y(ver.y), z(ver.z), w(1) {}
	float4(float3 ver, float w) : x(ver.x), y(ver.y), z(ver.z), w(w) {}

	float dot(const float4& other)
	{
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	float dot(const float3& other)
	{
		return x * other.x + y * other.y + z * other.z;
	}

	friend std::ostream& operator << (std::ostream& out, const float4& a)
	{
		out << a.x << " " << a.y << " " << a.z << " " << a.w << std::endl;
		return out;
	}

	float4 operator-()
	{
		return float4(-x, -y, -z);
	}
};

struct float4x4
{
	float4 mat[4];
	float4x4()
	{
		mat[0] = float4(0, 0, 0, 0);
		mat[1] = float4(0, 0, 0, 0);
		mat[2] = float4(0, 0, 0, 0);
		mat[3] = float4(0, 0, 0, 0);
	}

	float4x4(float4 w0, float4 w1, float4 w2, float4 w3)
	{
		mat[0] = w0;
		mat[1] = w1;
		mat[2] = w2;
		mat[3] = w3;
	}

	void transposit()
	{
		float tmp = mat[0].y;
		mat[0].y = mat[1].x;
		mat[1].x = tmp;
		tmp = mat[0].z;
		mat[0].z = mat[2].x;
		mat[2].x = tmp;
		tmp = mat[0].w;
		mat[0].w = mat[3].x;
		mat[3].x = tmp;

		tmp = mat[1].z;
		mat[1].z = mat[2].y;
		mat[2].y = tmp;
		tmp = mat[1].w;
		mat[1].w = mat[3].y;
		mat[3].y = tmp;

		tmp = mat[2].w;
		mat[2].w = mat[3].z;
		mat[3].z = tmp;
	}

	float4x4 transpositProd()
	{
		return float4x4(float4(mat[0].x, mat[1].x, mat[2].x, mat[3].x),
						float4(mat[0].y, mat[1].y, mat[2].y, mat[3].y),
						float4(mat[0].z, mat[1].z, mat[2].z, mat[3].z),
						float4(mat[0].w, mat[1].w, mat[2].w, mat[3].w));
	}

	float4x4 operator*(const float4x4& other)
	{
		return float4x4(
			float4
			(	mat[0].x * other[0].x + mat[0].y * other[1].x + mat[0].z * other[2].x + mat[0].w * other[3].x,
				mat[0].x * other[0].y + mat[0].y * other[1].y + mat[0].z * other[2].y + mat[0].w * other[3].y,
				mat[0].x * other[0].z + mat[0].y * other[1].z + mat[0].z * other[2].z + mat[0].w * other[3].z,
				mat[0].x * other[0].w + mat[0].y * other[1].w + mat[0].z * other[2].w + mat[0].w * other[3].w),
			float4
			(	mat[1].x * other[0].x + mat[1].y * other[1].x + mat[1].z * other[2].x + mat[1].w * other[3].x,
				mat[1].x * other[0].y + mat[1].y * other[1].y + mat[1].z * other[2].y + mat[1].w * other[3].y,
				mat[1].x * other[0].z + mat[1].y * other[1].z + mat[1].z * other[2].z + mat[1].w * other[3].z,
				mat[1].x * other[0].w + mat[1].y * other[1].w + mat[1].z * other[2].w + mat[1].w * other[3].w),
			float4
			(	mat[2].x * other[0].x + mat[2].y * other[1].x + mat[2].z * other[2].x + mat[2].w * other[3].x,
				mat[2].x * other[0].y + mat[2].y * other[1].y + mat[2].z * other[2].y + mat[2].w * other[3].y,
				mat[2].x * other[0].z + mat[2].y * other[1].z + mat[2].z * other[2].z + mat[2].w * other[3].z,
				mat[2].x * other[0].w + mat[2].y * other[1].w + mat[2].z * other[2].w + mat[2].w * other[3].w),
			float4
			(	mat[3].x * other[0].x + mat[3].y * other[1].x + mat[3].z * other[2].x + mat[3].w * other[3].x,
				mat[3].x * other[0].y + mat[3].y * other[1].y + mat[3].z * other[2].y + mat[3].w * other[3].y,
				mat[3].x * other[0].z + mat[3].y * other[1].z + mat[3].z * other[2].z + mat[3].w * other[3].z,
				mat[3].x * other[0].w + mat[3].y * other[1].w + mat[3].z * other[2].w + mat[3].w * other[3].w));
	}

	float4x4 combine(const float4x4& other)
	{
		return float4x4(
			float4
			(	mat[0].x + other[0].x,
				mat[0].y + other[1].y,
				mat[0].z + other[2].z,
				mat[0].w + other[3].w),
			float4
			(	mat[1].x + other[0].x,
				mat[1].y + other[1].y,
				mat[1].z + other[2].z,
				mat[1].w + other[3].w),
			float4
			(	mat[2].x + other[0].x,
				mat[2].y + other[1].y,
				mat[2].z + other[2].z,
				mat[2].w + other[3].w),
			float4
			(	mat[3].x + other[0].x,
				mat[3].y + other[1].y,
				mat[3].z + other[2].z,
				mat[3].w + other[3].w));
	}

	float4 operator*(const float4& other)
	{
		return float4(mat[0].x * other.x + mat[0].y * other.y + mat[0].z * other.z + mat[0].w * other.w,
			mat[1].x * other.x + mat[1].y * other.y + mat[1].z * other.z + mat[1].w * other.w,
			mat[2].x * other.x + mat[2].y * other.y + mat[2].z * other.z + mat[2].w * other.w,
			mat[3].x * other.x + mat[3].y * other.y + mat[3].z * other.z + mat[3].w * other.w);
	}

	float4 operator*(const float3& other)
	{
		return float4(mat[0].x * other.x + mat[0].y * other.y + mat[0].z * other.z + mat[0].w,
			mat[1].x * other.x + mat[1].y * other.y + mat[1].z * other.z + mat[1].w,
			mat[2].x * other.x + mat[2].y * other.y + mat[2].z * other.z + mat[2].w,
			mat[3].x * other.x + mat[3].y * other.y + mat[3].z * other.z + mat[3].w);
		//return float3(mat[0].x * other.x + mat[1].x * other.y + mat[2].x * other.z + mat[3].x,
		//	mat[0].y * other.x + mat[1].y * other.y + mat[2].y * other.z + mat[3].y,
		//	mat[0].z * other.x + mat[1].z * other.y + mat[2].z * other.z + mat[3].z);
	}

	float4& operator[](int i) { return mat[i]; }
	const float4& operator[](int i) const { return mat[i]; }
	friend std::ostream& operator << (std::ostream& out, const float4x4& a)
	{
		out << "[" << std::endl;
		out << a[0];
		out << a[1];
		out << a[2];
		out << a[3];
		out << "]" << std::endl;
		return out;
	}
};