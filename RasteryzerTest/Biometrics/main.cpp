#include <Windows.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Buffer.h"
#include "Structures.h"
#include "Rasterizer.h"
#include "VertexProcesor.h"
#include "ObjParser.h"
#include "Object.h"

int main()
{
	// Create black empty images
	cv::Mat image = cv::Mat::zeros(HEIGHT - 1, WIDTH - 1, CV_8UC3);
	std::string winName = "Image";

	Object obj;
	ObjParser::readMeshFromFile("spherePro.obj",&obj);
	obj.calculateNormals();
	std::vector<Object> objVector;
	objVector.push_back(obj);

	DirectionalLight light = DirectionalLight(float3(1, 1, 1), float3(0.1, 0.1, 0.1), float3(0.5, 0.5, 0.5), float3(0.5, 0.5, 0.5), 0.00000001f);

	Rasterizer rasterizer(&objVector, &light);

	float3 move = float3(0.0f, 0.0f, 0.f);
	float3 axis = float3(0.f, 0.f, 0.f);
	float angle = 0.f;
	float3 scale = float3(1.0f, 1.0f, 1.0f);
	rasterizer.objects.at(0).verProc.multByTranslation(move);
	rasterizer.objects.at(0).verProc.multByRotation(angle, axis);
	rasterizer.objects.at(0).verProc.multByScale(scale);
	rasterizer.objects.at(0).verProc.obj2world.transposit();

	float3 eye = float3(0.0f, 0.0f, -2.0f);
	float3 center = eye + float3(0.0f, 0.0f, 1.0f);
	float3 up = float3(0, 1, 0);
	rasterizer.objects.at(0).verProc.setLookat(eye, center, up);
	rasterizer.objects.at(0).verProc.world2view.transposit();

	float fovyV = 90.0f;
	float aspectV = 1.0f;
	float nearV = 0.1f;
	float farV = 100.0f;
	rasterizer.objects.at(0).verProc.setPerspective(fovyV, aspectV, nearV, farV);
	rasterizer.objects.at(0).verProc.view2proj.transposit();

	rasterizer.rasterize();
	rasterizer.getImage(&image);
	cv::namedWindow(winName, cv::WindowFlags::WINDOW_NORMAL);
	imshow(winName, image);
	cv::waitKey(0);

	auto oldTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> deltaTime = std::chrono::high_resolution_clock::now() - oldTime;
	float dTime = deltaTime.count();
	float fps;

	cv::imwrite("image.jpg", image);
	//int iterator = 0;

	while (cv::getWindowProperty(winName, 0) >= 0)
	{
		deltaTime = std::chrono::high_resolution_clock::now() - oldTime;
		oldTime = std::chrono::high_resolution_clock::now();
		dTime = deltaTime.count();
		fps = 1.0f / dTime;

		rasterizer.objects.at(0).verProc.clear();
		//move = float3(0.0f, 0.8f, 2.f);
		axis = float3(1.f, 1.f, 1.f);
		angle += 90.0f * dTime;
		//scale = float3(1.f, 1.f, 1.f);
		rasterizer.objects.at(0).verProc.multByTranslation(move);
		rasterizer.objects.at(0).verProc.multByRotation(angle, axis);
		rasterizer.objects.at(0).verProc.multByScale(scale);
		rasterizer.objects.at(0).verProc.obj2world.transposit();

		//eye = float3(0.0f, 1.0f, 0.0f);
		//center = eye + float3(0.0f, 0.0f, 1.0f);
		//up = float3(0, 1, 0);
		rasterizer.objects.at(0).verProc.setLookat(eye, center, up);
		rasterizer.objects.at(0).verProc.world2view.transposit();

		//float fovyV = 90.0f;
		//float aspectV = 1.0f;
		//nearV += 0.5f * dTime;
		//farV -= 0.5f * dTime;
		rasterizer.objects.at(0).verProc.setPerspective(fovyV, aspectV, nearV, farV);
		rasterizer.objects.at(0).verProc.view2proj.transposit();

		
		rasterizer.rasterize();
		rasterizer.getImage(&image);
		cv::putText(image,std::to_string(fps),
					cv::Point(10, 30), //top-left position
					cv::FONT_HERSHEY_DUPLEX,
					1.0,
					CV_RGB(255, 255, 255), //font color
					2);
		imshow(winName, image);
		//cv::imwrite("image"+std::to_string(iterator) + ".jpg", image);
		//iterator++;
		light.shininess += 0.1f;
		cv::waitKey(1);
	}

	return(0);
}