#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Buffer.h"
#include "Structures.h"

float3 InWindow(float3* ver)
{
	return float3((ver->x + 1) * WIDTH * 0.5f, (ver->y + 1) * HEIGHT * 0.5f, (ver->z + 1) * DEPTH * 0.5f);
}

void Triangulate(Buffer* buffer, float3* ver1W, float3* ver2W, float3* ver3W)
{
	unsigned char r1(255), r2(0), r3(0);
	unsigned char g1(0), g2(255), g3(0);
	unsigned char b1(0), b2(0), b3(255);
	for (int y = buffer->miny; y < buffer->maxy; y++)
	{
		for (int x = buffer->minx; x < buffer->maxx; x++)
		{
			//if (((ver1W->x - ver2W->x) * (y - ver1W->y) - (ver1W->y - ver2W->y) * (x - ver1W->x)) > 0 &&
			//	((ver2W->x - ver3W->x) * (y - ver2W->y) - (ver2W->y - ver3W->y) * (x - ver2W->x)) > 0 &&
			//	((ver3W->x - ver1W->x) * (y - ver3W->y) - (ver3W->y - ver1W->y) * (x - ver3W->x)) > 0)
			if ((((ver1W->x - ver2W->x) * (y - ver1W->y) - (ver1W->y - ver2W->y) * (x - ver1W->x) > 0) ||
				((buffer->tl1 && (((ver1W->x - ver2W->x) * (y - ver1W->y) - (ver1W->y - ver2W->y) * (x - ver1W->x)) >= 0)))) &&
				(((ver2W->x - ver3W->x) * (y - ver2W->y) - (ver2W->y - ver3W->y) * (x - ver2W->x) > 0) ||
				((buffer->tl2 && (((ver2W->x - ver3W->x) * (y - ver2W->y) - (ver2W->y - ver3W->y) * (x - ver2W->x)) >= 0)))) &&
				(((ver3W->x - ver1W->x) * (y - ver3W->y) - (ver3W->y - ver1W->y) * (x - ver3W->x) > 0) ||
				((buffer->tl3 && (((ver3W->x - ver1W->x) * (y - ver3W->y) - (ver3W->y - ver1W->y) * (x - ver3W->x)) >= 0)))))
			{
				float lamb1 = (buffer->dy23 * (x - ver3W->x) + (ver3W->x - ver2W->x) * (y - ver3W->y)) / (buffer->dy23 * (ver1W->x - ver3W->x) + (ver3W->x - ver2W->x) * (ver1W->y - ver3W->y));
				float lamb2 = ((buffer->dy31 * (x - ver3W->x) + (ver1W->x - ver3W->x) * (y - ver3W->y)) / (buffer->dy31 * buffer->dx23 + (ver1W->x - ver3W->x) * buffer->dy23));
				float lamb3 = 1 - lamb1 - lamb2;
				unsigned char r = (r1 * lamb1 + r2 * lamb2 + r3 * lamb3);
				unsigned char g = (g1 * lamb1 + g2 * lamb2 + g3 * lamb3);
				unsigned char b = (b1 * lamb1 + b2 * lamb2 + b3 * lamb3);
				float depth = lamb1 * ver1W->z + lamb2 * ver2W->z + lamb3 * ver3W->z;
				if (depth < buffer->depth[x][y])
				{
					buffer->pixels[x][y] = b | g << 8 | r << 16;
					buffer->depth[x][y] = depth;
				}				
			}
		}
	}
}

int main()
{
	// Create black empty images
	cv::Mat image = cv::Mat::zeros(HEIGHT - 1, WIDTH - 1, CV_8UC3);
	Buffer buffer;
	buffer.setSize(HEIGHT, WIDTH);
	buffer.clearColor(100, 100, 100);
	buffer.clearDepth();

	float3 ver3(-0.5f, -0.5f, 0.8f);
	float3 ver2(-0.5f, 0.5f, 0.8f);
	float3 ver1(0.4f, 0.0f, 0.0f);

	//float3 ver3(-0.5f, -0.5f, 0.3f);
	//float3 ver2(-0.5f, 0.5f, 0.3f);
	//float3 ver1(0.5f, 0.5f, 0.3f);

	float3 ver6(-0.5f, -0.5f, 0.30f);
	float3 ver5(0.5f, -0.5f, 0.30f);
	float3 ver4(0.5f, 0.5f, 0.30f);

	float3 ver3W = InWindow(&ver3);
	float3 ver2W = InWindow(&ver2);
	float3 ver1W = InWindow(&ver1);

	float3 ver6W = InWindow(&ver6);
	float3 ver5W = InWindow(&ver5);
	float3 ver4W = InWindow(&ver4);

	buffer.setBordersAndCons(&ver3W, &ver2W, &ver1W);
	buffer.culling();
	Triangulate(&buffer, &ver3W, &ver2W, &ver1W);

	buffer.setBordersAndCons(&ver4W, &ver5W, &ver6W);
	buffer.culling();
	Triangulate(&buffer, &ver4W, &ver5W, &ver6W);

	for (int i = 0; i < HEIGHT - 1; i++)
	{
		for (int j = 0; j < WIDTH - 1; j++)
		{
			cv::Vec3b col;
			unsigned int pixel = buffer.pixels[j][i];
			col.val[0] = (unsigned char) pixel;
			pixel = pixel >> 8;
			col.val[1] = (unsigned char) pixel;
			pixel = pixel >> 8;
			col.val[2] = (unsigned char) pixel;
			image.at<cv::Vec3b>(HEIGHT - 2 - i, j) = col;
			// "HEIGHT - 2 - i" is translation to openCV, where point (0,0) is left top corner
		}
	}
	
	imshow("Image", image);
	cv::waitKey(0);
	cv::imwrite("image.jpg", image);
	return(0);
}