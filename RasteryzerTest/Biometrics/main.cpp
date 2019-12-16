#include <Windows.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Buffer.h"
#include "Rasterizer.h"
#include "VertexProcesor.h"
#include "ObjParser.h"
#include "Object.h"
#include "float3.h"
#include "Constant.h"

int main()
{
	// Create black empty images
	cv::Mat image = cv::Mat::zeros(HEIGHT - 1, WIDTH - 1, CV_8UC3);
	std::string winName = "Image";

	float3 red = float3(1.0, 0.0, 0.0);
	float3 shipColor = float3(1.0, 0.5, 0.0);
	Object obj(&shipColor);
	ObjParser::readMeshFromFile("shipMobile.obj",&obj);
	obj.calculateNormals();
	Object obj2(&red);
	ObjParser::readMeshFromFile("sphere.obj", &obj2);
	obj2.calculateNormals();
	std::vector<Object> objVector;
	objVector.push_back(obj);
	objVector.push_back(obj2);

	DirectionalLight light = DirectionalLight(float3(0, 1, 1), float3(0.1, 0.1, 0.1), float3(0.5, 0.5, 0.5), float3(0.5, 0.5, 0.5), 40.0f);

	float3 eye = float3(0.0f, 1.0f, 0.0f);
	Rasterizer rasterizer(&objVector, &light, &eye);

	float3 move = float3(0.0f, 0.8f, 6.0f);
	float3 axis = float3(0.f, 0.f, 0.f);
	float angle = 0.f;
	float3 scale = float3(1.0f, 1.0f, 1.0f);
	rasterizer.objects.at(0).verProc.multByRotation(angle, axis);
	rasterizer.objects.at(0).verProc.multByTranslation(move);
	rasterizer.objects.at(0).verProc.multByScale(scale);
	rasterizer.objects.at(0).verProc.obj2world.transposit();
	rasterizer.objects.at(0).verProc.obj2worldRotation.transposit();

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

	move = float3(4.5f, 4.5f, 6.0f);
	axis = float3(0.f, 0.f, 0.f);
	angle = 0.f;
	scale = float3(1.0f, 1.0f, 1.0f);
	rasterizer.objects.at(1).verProc.multByRotation(angle, axis);
	rasterizer.objects.at(1).verProc.multByTranslation(move);
	rasterizer.objects.at(1).verProc.multByScale(scale);
	rasterizer.objects.at(1).verProc.obj2world.transposit();
	rasterizer.objects.at(1).verProc.obj2worldRotation.transposit();

	rasterizer.objects.at(1).verProc.setLookat(eye, center, up);
	rasterizer.objects.at(1).verProc.world2view.transposit();

	rasterizer.objects.at(1).verProc.setPerspective(fovyV, aspectV, nearV, farV);
	rasterizer.objects.at(1).verProc.view2proj.transposit();

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
	int iterator = 0;
	bool fr = false, fb = true, fg = true;

	while (cv::getWindowProperty(winName, 0) >= 0)
	{
		if (shipColor.x > 1)
			fr = false;
		if (shipColor.y > 1)
			fb = false;
		if (shipColor.z > 1)
			fg = false;
		if (shipColor.x < 0)
			fr = true;
		if (shipColor.y < 0)
			fb = true;
		if (shipColor.z < 0)
			fg = true;
		if (fr)
			shipColor.x += 0.01;
		else
			shipColor.x -= 0.01;
		if (fb)
			shipColor.y += 0.01;
		else
			shipColor.y -= 0.01;
		if (fg)
			shipColor.z += 0.01;
		else
			shipColor.z -= 0.01;
		deltaTime = std::chrono::high_resolution_clock::now() - oldTime;
		oldTime = std::chrono::high_resolution_clock::now();
		dTime = deltaTime.count();
		fps = 1.0f / dTime;

		rasterizer.objects.at(0).verProc.clear();
		move = float3(0.0f, 0.8f, 6.0f);
		axis = float3(1.f, 1.f, 1.f);
		angle += 90.0f * dTime;
		scale = float3(1.f, 1.f, 1.f);
		rasterizer.objects.at(0).verProc.multByTranslation(move);
		rasterizer.objects.at(0).verProc.multByRotation(angle, axis);
		rasterizer.objects.at(0).verProc.multByScale(scale);
		rasterizer.objects.at(0).verProc.obj2world.transposit();
		rasterizer.objects.at(0).verProc.obj2worldRotation.transposit();

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
		cv::imwrite("image"+std::to_string(iterator) + ".jpg", image);
		iterator++;
		cv::waitKey(1);
	}

	return(0);
}