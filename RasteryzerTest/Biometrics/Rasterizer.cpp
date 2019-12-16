#include "Rasterizer.h"

float3 InWindow(float4* ver)
{
	return float3((ver->x + 1) * WIDTH * 0.5f, (ver->y + 1) * HEIGHT * 0.5f, ver->z); //+ 1) * DEPTH * 0.5f);
}

float3 InWindow(float3* ver)
{
	return float3((ver->x + 1) * WIDTH * 0.5f, (ver->y + 1) * HEIGHT * 0.5f, ver->z); //+ 1) * DEPTH * 0.5f);
}

void Rasterizer::rasterize()
{
	buffer.clearColor(0, 0, 0);
	buffer.clearDepth();
	applayMatrices();
	for (int j = 0; j < objectIndexes.size(); ++j)
	{
		int start; 
		if (j == 0)
		{
			start = 0;
		} 
		else
		{
			start = objects.at(j - 1).triangleIndexes.size();
		}
		int end = start + objects.at(j).triangleIndexes.size();
		for (int i = start; i < end; i += 3)
		{
			float3 pos3 = vertices.at(triangleIndexes.at(i));
			float3 pos2 = vertices.at(triangleIndexes.at(i + 1));
			float3 pos1 = vertices.at(triangleIndexes.at(i + 2));

			float3 n3 = normals.at(triangleIndexes.at(i));
			float3 n2 = normals.at(triangleIndexes.at(i + 1));
			float3 n1 = normals.at(triangleIndexes.at(i + 2));

			float3 viewDir3 = (*eye - pos3).normalizeProd();
			float3 viewDir2 = (*eye - pos2).normalizeProd();
			float3 viewDir1 = (*eye - pos1).normalizeProd();

			float3 lightVector3 = (float3(light->position) - pos3).normalizeProd();
			float3 lightVector2 = (float3(light->position) - pos2).normalizeProd();
			float3 lightVector1 = (float3(light->position) - pos1).normalizeProd();

			float diffuse3 = fmax(0.0f, n3.dot(lightVector3));
			float diffuse2 = fmax(0.0f, n2.dot(lightVector2));
			float diffuse1 = fmax(0.0f, n1.dot(lightVector1));

			float3 angleW3 = (viewDir3 + lightVector3).normalizeProd();
			float3 angleW2 = (viewDir2 + lightVector2).normalizeProd();
			float3 angleW1 = (viewDir1 + lightVector1).normalizeProd();

			float specular3 = fmax(0.0f, n3.dot(angleW3));
			float specular2 = fmax(0.0f, n2.dot(angleW2));
			float specular1 = fmax(0.0f, n1.dot(angleW1));

			specular3 = pow(specular3, fmax(1.0f, light->shininess)); //* 2.0f;
			specular2 = pow(specular2, fmax(1.0f, light->shininess)); //* 2.0f;
			specular1 = pow(specular1, fmax(1.0f, light->shininess)); //* 2.0f;

			float3 col3 = light->ambient + light->diffuse * diffuse3 + light->specular * specular3;
			float3 col2 = light->ambient + light->diffuse * diffuse2 + light->specular * specular2;
			float3 col1 = light->ambient + light->diffuse * diffuse1 + light->specular * specular1;
			col3 = *objects.at(j).col * col3;
			col2 = *objects.at(j).col * col2;
			col1 = *objects.at(j).col * col1;
			col3.ZeroOne();
			col2.ZeroOne();
			col1.ZeroOne();

			float4 posInCam3 = vertices.at(triangleIndexes.at(i));
			float4 posInCam2 = vertices.at(triangleIndexes.at(i + 1));
			float4 posInCam1 = vertices.at(triangleIndexes.at(i + 2));

			posInCam3 = objects.at(j).verProc.world2view * posInCam3;
			posInCam2 = objects.at(j).verProc.world2view * posInCam2;
			posInCam1 = objects.at(j).verProc.world2view * posInCam1;

			posInCam3 = objects.at(j).verProc.view2proj * posInCam3;
			posInCam2 = objects.at(j).verProc.view2proj * posInCam2;
			posInCam1 = objects.at(j).verProc.view2proj * posInCam1;

			float w3 = posInCam3.w;
			float w2 = posInCam2.w;
			float w1 = posInCam1.w;

			posInCam3 = float4(posInCam3.x / w3, posInCam3.y / w3, posInCam3.z / w3, w3);
			posInCam2 = float4(posInCam2.x / w2, posInCam2.y / w2, posInCam2.z / w2, w2);
			posInCam1 = float4(posInCam1.x / w1, posInCam1.y / w1, posInCam1.z / w1, w1);			

			float3 ver3W = InWindow(&posInCam3);
			float3 ver2W = InWindow(&posInCam2);
			float3 ver1W = InWindow(&posInCam1);

			buffer.setBordersAndCons(&ver3W, &ver2W, &ver1W);
			buffer.culling();
			triangulate(&ver3W, &ver2W, &ver1W, &col3, &col2, &col1);
		}
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

Rasterizer::Rasterizer(std::vector<Object>* objects, DirectionalLight* light, float3* eye)
{
	buffer.setSize(HEIGHT, WIDTH);

	this->objects = *objects;
	this->light = light;
	this->eye = eye;
	setUpVerticlesOfObjects();
}

void Rasterizer::triangulate(float3* ver1W, float3* ver2W, float3* ver3W, float3* col1, float3* col2, float3* col3)
{
	for (int y = buffer.miny; y <= buffer.maxy; y++)
	{
		for (int x = buffer.minx; x <= buffer.maxx; x++)
		{
			if (((					(ver1W->x - ver2W->x) * (y - ver1W->y) - (ver1W->y - ver2W->y) * (x - ver1W->x) > 0) ||
				((buffer.tl1 && ((	(ver1W->x - ver2W->x) * (y - ver1W->y) - (ver1W->y - ver2W->y) * (x - ver1W->x)) >= 0)))) &&
				((					(ver2W->x - ver3W->x) * (y - ver2W->y) - (ver2W->y - ver3W->y) * (x - ver2W->x) > 0) ||
				((buffer.tl2 && ((	(ver2W->x - ver3W->x) * (y - ver2W->y) - (ver2W->y - ver3W->y) * (x - ver2W->x)) >= 0)))) &&
				((					(ver3W->x - ver1W->x) * (y - ver3W->y) - (ver3W->y - ver1W->y) * (x - ver3W->x) > 0) ||
				((buffer.tl3 && ((	(ver3W->x - ver1W->x) * (y - ver3W->y) - (ver3W->y - ver1W->y) * (x - ver3W->x)) >= 0)))))
			{
				//if ((ver1W->x - ver2W->x) * (y - ver1W->y) - (ver1W->y - ver2W->y) * (x - ver1W->x) < 10.0f ||
				//	(ver2W->x - ver3W->x) * (y - ver2W->y) - (ver2W->y - ver3W->y) * (x - ver2W->x) < 10.0f ||
				//	(ver3W->x - ver1W->x) * (y - ver3W->y) - (ver3W->y - ver1W->y) * (x - ver3W->x) < 10.0f)
				{
					float lamb1 = ((buffer.dy23 * (x - ver3W->x)) + (ver3W->x - ver2W->x) * (y - ver3W->y)) / ((buffer.dy23 * (ver1W->x - ver3W->x)) + (ver3W->x - ver2W->x) * (ver1W->y - ver3W->y));
					float lamb2 = ((buffer.dy31 * (x - ver3W->x)) + (ver1W->x - ver3W->x) * (y - ver3W->y)) / ((buffer.dy31 * buffer.dx23) + ((ver1W->x - ver3W->x) * buffer.dy23));
					float lamb3 = 1 - lamb1 - lamb2;
					unsigned char r = 255 * (col1->x * lamb1 + col2->x * lamb2 + col3->x * lamb3);
					unsigned char g = 255 * (col1->y * lamb1 + col2->y * lamb2 + col3->y * lamb3);
					unsigned char b = 255 * (col1->z * lamb1 + col2->z * lamb2 + col3->z * lamb3);
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
}

void Rasterizer::applayMatrices()
{
	for (int i = 0; i < objectIndexes.size(); ++i)
	{
		int start = objectIndexes.at(i);
		int end;
		if (i + 1 < objectIndexes.size())
			end = objectIndexes.at(i + 1);
		else
			end = vertices.size();
		for (int j = start; j < end; ++j)
		{
			vertices.at(j) = objects.at(i).orginalVertices.at(j - start);
			vertices.at(j) = objects.at(i).verProc.obj2world * vertices.at(j);
			//vertices.at(j) = objects.at(i).verProc.world2view * vertices.at(j);
			//vertices.at(j) = objects.at(i).verProc.view2proj * vertices.at(j);
			float w = vertices.at(j).w;
			vertices.at(j) = float4(vertices.at(j).x / w, vertices.at(j).y / w, vertices.at(j).z / w, 1);

			normals.at(j) = objects.at(i).normals.at(j - start);
			normals.at(j) = objects.at(i).verProc.obj2worldRotation * normals.at(j);
			//normals.at(j).normalize();
			//w = normals.at(j).w;
			//normals.at(j) = float4(normals.at(j).x / w, normals.at(j).y / w, normals.at(j).z / w, w);
		}
	}
}

void Rasterizer::setUpVerticlesOfObjects()
{
	for (int i = 0; i < objects.size(); i++)
	{
		Object* obj = &objects.at(i);
		int offset = vertices.size();
		objectIndexes.push_back(offset);
		for (int j = 0; j < obj->orginalVertices.size(); j++)
		{
			vertices.push_back(float4(obj->orginalVertices.at(j)));
		}
		for (int j = 0; j < obj->normals.size(); j++)
		{
			normals.push_back(float4(obj->normals.at(j)));
		}
		for (int j = 0; j < obj->triangleIndexes.size(); j++)
		{
			triangleIndexes.push_back(obj->triangleIndexes.at(j) + offset);
		}
	}
}
