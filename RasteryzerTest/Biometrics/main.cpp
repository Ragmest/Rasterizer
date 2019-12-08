#include <Windows.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Buffer.h"
#include "Structures.h"
#include "Rasterizer.h"
#include "VertexProcesor.h"

int main()
{
	// Create black empty images
	cv::Mat image = cv::Mat::zeros(HEIGHT - 1, WIDTH - 1, CV_8UC3);
	std::string winName = "Image";

	Rasterizer rasterizer;

	float3 move = float3(0.f, 0.8f, 2.f);
	float3 axis = float3(0.f, 0.f, 0.f);
	float angle = 0.f;
	float3 scale = float3(1.f, 1.f, 1.f);
	rasterizer.vertProc.multByScale(scale);
	rasterizer.vertProc.multByRotation(angle, axis);
	rasterizer.vertProc.multByTranslation(move);
	rasterizer.vertProc.obj2world.transposit();

	float3 eye = float3(0.0f, 1.0f, 0.0f);
	float3 center = eye + float3(0.0f, 0.0f, 1.0f);
	float3 up = float3(0, 1, 0);
	rasterizer.vertProc.setLookat(eye, center, up);
	rasterizer.vertProc.world2view.transposit();

	float fovyV = 90.0f;
	float aspectV = 1.0f;
	float nearV = 0.0f;
	float farV = 5.0f;
	rasterizer.vertProc.setPerspective(fovyV, aspectV, nearV, farV);
	rasterizer.vertProc.view2proj.transposit();

	rasterizer.rasterize();
	rasterizer.getImage(&image);
	cv::namedWindow(winName, cv::WindowFlags::WINDOW_NORMAL);
	imshow(winName, image);
	cv::waitKey(0);

	auto oldTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> deltaTime = std::chrono::high_resolution_clock::now() - oldTime;
	float dTime = deltaTime.count();
	float fps;
	while (cv::getWindowProperty(winName, 0) >= 0)
	{
		deltaTime = std::chrono::high_resolution_clock::now() - oldTime;
		oldTime = std::chrono::high_resolution_clock::now();
		dTime = deltaTime.count();
		fps = 1.0f / dTime;

		rasterizer.vertProc.clear();
		//move = float3(0.0f, 0.8f, 2.f);
		axis = float3(0.f, 0.f, 1.f);
		angle += 90.0f * dTime;
		//scale = float3(1.f, 1.f, 1.f);
		rasterizer.vertProc.multByScale(scale);
		rasterizer.vertProc.multByTranslation(move);
		rasterizer.vertProc.multByRotation(angle, axis);
		rasterizer.vertProc.obj2world.transposit();

		//float3 eye = float3(0.0f, 1.0f, 0.0f);
		//float3 center = eye + float3(0.0f, 0.0f, 1.0f);
		//float3 up = float3(0, 1, 0);
		rasterizer.vertProc.setLookat(eye, center, up);
		rasterizer.vertProc.world2view.transposit();

		//float fovyV = 90.0f;
		//float aspectV = 1.0f;
		//float nearV = 0.0f;
		//float farV = 5.0f;
		rasterizer.vertProc.setPerspective(fovyV, aspectV, nearV, farV);
		rasterizer.vertProc.view2proj.transposit();

		
		rasterizer.rasterize();
		rasterizer.getImage(&image);
		cv::putText(image,std::to_string(fps),
					cv::Point(10, 30), //top-left position
					cv::FONT_HERSHEY_DUPLEX,
					1.0,
					CV_RGB(255, 255, 255), //font color
					2);
		imshow(winName, image);
		cv::waitKey(1);
	}
	cv::imwrite("image.jpg", image);

	return(0);
}