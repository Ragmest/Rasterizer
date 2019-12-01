#include "Rasterizer.h"

float3 InWindow(float3* ver)
{
	return float3((ver->x + 1) * WIDTH * 0.5f, (ver->y + 1) * HEIGHT * 0.5f, (ver->z + 1) * DEPTH * 0.5f);
}

void Rasterizer::Rasterize()
{
	int size = triangleIndexes.size();
	for (int i = 0; i < size; i += 3)
	{
		float3 tmp3 = triangles.at(triangleIndexes.at(i));
		float3 tmp2 = triangles.at(triangleIndexes.at(i + 1));
		float3 tmp1 = triangles.at(triangleIndexes.at(i + 2));

		float3 ver3W = InWindow(&triangles.at(triangleIndexes.at(i)));
		float3 ver2W = InWindow(&triangles.at(triangleIndexes.at(i + 1)));
		float3 ver1W = InWindow(&triangles.at(triangleIndexes.at(i + 2)));
		buffer.setBordersAndCons(&ver3W, &ver2W, &ver1W);
		buffer.culling();
		Triangulate(&ver3W, &ver2W, &ver1W);
	}
}

void Rasterizer::getImage(cv::Mat* image)
{
	for (int i = 0; i < HEIGHT - 1; i++)
	{
		for (int j = 0; j < WIDTH - 1; j++)
		{
			cv::Vec3b col;
			unsigned int pixel = buffer.pixels[j][i];
			col.val[0] = (unsigned char)pixel;
			pixel = pixel >> 8;
			col.val[1] = (unsigned char)pixel;
			pixel = pixel >> 8;
			col.val[2] = (unsigned char)pixel;
			image->at<cv::Vec3b>(HEIGHT - 2 - i, j) = col;
			// "HEIGHT - 2 - i" is translation to openCV, where point (0,0) is left top corner
		}
	}
}

void Rasterizer::Triangulate(float3* ver1W, float3* ver2W, float3* ver3W)
{
	unsigned char r1(255), r2(0), r3(0);
	unsigned char g1(0), g2(255), g3(0);
	unsigned char b1(0), b2(0), b3(255);
	for (int y = buffer.miny; y < buffer.maxy; y++)
	{
		for (int x = buffer.minx; x < buffer.maxx; x++)
		{
			//if (((ver1W->x - ver2W->x) * (y - ver1W->y) - (ver1W->y - ver2W->y) * (x - ver1W->x)) > 0 &&
			//	((ver2W->x - ver3W->x) * (y - ver2W->y) - (ver2W->y - ver3W->y) * (x - ver2W->x)) > 0 &&
			//	((ver3W->x - ver1W->x) * (y - ver3W->y) - (ver3W->y - ver1W->y) * (x - ver3W->x)) > 0)
			if ((((ver1W->x - ver2W->x) * (y - ver1W->y) - (ver1W->y - ver2W->y) * (x - ver1W->x) > 0) ||
				((buffer.tl1 && (((ver1W->x - ver2W->x) * (y - ver1W->y) - (ver1W->y - ver2W->y) * (x - ver1W->x)) >= 0)))) &&
				(((ver2W->x - ver3W->x) * (y - ver2W->y) - (ver2W->y - ver3W->y) * (x - ver2W->x) > 0) ||
				((buffer.tl2 && (((ver2W->x - ver3W->x) * (y - ver2W->y) - (ver2W->y - ver3W->y) * (x - ver2W->x)) >= 0)))) &&
					(((ver3W->x - ver1W->x) * (y - ver3W->y) - (ver3W->y - ver1W->y) * (x - ver3W->x) > 0) ||
				((buffer.tl3 && (((ver3W->x - ver1W->x) * (y - ver3W->y) - (ver3W->y - ver1W->y) * (x - ver3W->x)) >= 0)))))
			{
				float lamb1 = (buffer.dy23 * (x - ver3W->x) + (ver3W->x - ver2W->x) * (y - ver3W->y)) / (buffer.dy23 * (ver1W->x - ver3W->x) + (ver3W->x - ver2W->x) * (ver1W->y - ver3W->y));
				float lamb2 = ((buffer.dy31 * (x - ver3W->x) + (ver1W->x - ver3W->x) * (y - ver3W->y)) / (buffer.dy31 * buffer.dx23 + (ver1W->x - ver3W->x) * buffer.dy23));
				float lamb3 = 1 - lamb1 - lamb2;
				unsigned char r = (r1 * lamb1 + r2 * lamb2 + r3 * lamb3);
				unsigned char g = (g1 * lamb1 + g2 * lamb2 + g3 * lamb3);
				unsigned char b = (b1 * lamb1 + b2 * lamb2 + b3 * lamb3);
				float depth = lamb1 * ver1W->z + lamb2 * ver2W->z + lamb3 * ver3W->z;
				if (depth < buffer.depth[x][y])
				{
					buffer.pixels[x][y] = b | g << 8 | r << 16;
					buffer.depth[x][y] = depth;
				}
			}
		}
	}
}

Rasterizer::Rasterizer()
{
	buffer.setSize(HEIGHT, WIDTH);
	buffer.clearColor(100, 100, 100);
	buffer.clearDepth();

	//triangles.push_back(float3(-0.5f, -0.5f, 0.8f));
	//triangles.push_back(float3(-0.5f, 0.5f, 0.8f));
	//triangles.push_back(float3(0.4f, 0.0f, 0.0f));
	triangles.push_back(float3(-0.5f, -0.5f, 0.3f));
	triangles.push_back(float3(-0.5f, 0.5f, 0.3f));
	triangles.push_back(float3(0.5f, 0.5f, 0.3f));
	triangleIndexes.push_back(0);
	triangleIndexes.push_back(1);
	triangleIndexes.push_back(2);

	triangles.push_back(float3(-0.5f, -0.5f, 0.3f));
	triangles.push_back(float3(0.5f, -0.5f, 0.3f));
	triangles.push_back(float3(0.5f, 0.5f, 0.3f));
	triangleIndexes.push_back(5);
	triangleIndexes.push_back(4);
	triangleIndexes.push_back(3);
}
