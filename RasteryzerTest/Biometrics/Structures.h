#pragma once
#include <iostream>
#define WIDTH 1024	
#define HEIGHT 1024
#define DEPTH 1024
#define PI 3.14159

const float epsilon = 0.0001f;

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
		return x * other.x + y * other.y + z * other.z;
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
		return *this - (normal *( 2 * this->dot(normal)));
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

	float dot(const float4& other)
	{
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	friend std::ostream& operator << (std::ostream& out, const float4& a)
	{
		out << a.x << " " << a.y << " " << a.z << " " << a.w << std::endl;
		return out;
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

	float4 operator*(const float4& other)
	{
		return float4(	mat[0].x * other.x + mat[0].y * other.y + mat[0].z * other.z + mat[0].w * other.w,
						mat[1].x * other.x + mat[1].y * other.y + mat[1].z * other.z + mat[1].w * other.w,
						mat[2].x * other.x + mat[2].y * other.y + mat[2].z * other.z + mat[2].w * other.w,
						mat[3].x * other.x + mat[3].y * other.y + mat[3].z * other.z + mat[3].w * other.w);
	}

	float4 operator*(const float3& other)
	{
		return float4(	mat[0].x * other.x + mat[0].y * other.y + mat[0].z * other.z + mat[0].w,
						mat[1].x * other.x + mat[1].y * other.y + mat[1].z * other.z + mat[1].w,
						mat[2].x * other.x + mat[2].y * other.y + mat[2].z * other.z + mat[2].w,
						mat[3].x * other.x + mat[3].y * other.y + mat[3].z * other.z + mat[3].w);
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