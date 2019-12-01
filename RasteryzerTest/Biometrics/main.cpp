#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Buffer.h"
#include "Structures.h"
#include "Rasterizer.h"

int main()
{
	// Create black empty images
	cv::Mat image = cv::Mat::zeros(HEIGHT - 1, WIDTH - 1, CV_8UC3);

	Rasterizer rasterizer;
	rasterizer.Rasterize();
	rasterizer.getImage(&image);
	imshow("Image", image);
	cv::waitKey(0);
	cv::imwrite("image.jpg", image);

	return(0);
}