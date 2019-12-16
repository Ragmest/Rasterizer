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
	int size = triangleIndexes.size();
	for (int i = 0; i < size; i += 3)
	{
		float3 ver3W = InWindow(&vertices.at(triangleIndexes.at(i)));
		float3 ver2W = InWindow(&vertices.at(triangleIndexes.at(i + 1)));
		float3 ver1W = InWindow(&vertices.at(triangleIndexes.at(i + 2)));
		float3 v3 = -float3(vertices.at(triangleIndexes.at(i)).x, vertices.at(triangleIndexes.at(i)).y, vertices.at(triangleIndexes.at(i)).z);
		float3 v2 = -float3(vertices.at(triangleIndexes.at(i + 1)).x, vertices.at(triangleIndexes.at(i + 1)).y, vertices.at(triangleIndexes.at(i + 1)).z);
		float3 v1 = -float3(vertices.at(triangleIndexes.at(i + 2)).x, vertices.at(triangleIndexes.at(i + 2)).y, vertices.at(triangleIndexes.at(i + 2)).z);
		//float3 v3 = float3(verticesMinus.at(triangleIndexes.at(i)).x, verticesMinus.at(triangleIndexes.at(i)).y, verticesMinus.at(triangleIndexes.at(i)).z);
		//float3 v2 = float3(verticesMinus.at(triangleIndexes.at(i + 1)).x, verticesMinus.at(triangleIndexes.at(i + 1)).y, verticesMinus.at(triangleIndexes.at(i + 1)).z);
		//float3 v1 = float3(verticesMinus.at(triangleIndexes.at(i + 2)).x, verticesMinus.at(triangleIndexes.at(i + 2)).y, verticesMinus.at(triangleIndexes.at(i + 2)).z);
		v3.normalize();
		v2.normalize();
		v1.normalize();
		float3 n3 = float3(normals.at(triangleIndexes.at(i)).x, normals.at(triangleIndexes.at(i)).y, normals.at(triangleIndexes.at(i)).z);
		float3 n2 = float3(normals.at(triangleIndexes.at(i + 1)).x, normals.at(triangleIndexes.at(i + 1)).y, normals.at(triangleIndexes.at(i + 1)).z);
		float3 n1 = float3(normals.at(triangleIndexes.at(i + 2)).x, normals.at(triangleIndexes.at(i + 2)).y, normals.at(triangleIndexes.at(i + 2)).z);
		n3.normalize();
		n2.normalize();
		n1.normalize();

		float3 posLight = float3(light->position.x, light->position.y, light->position.z);
		posLight.normalize();

		float3 R1 = n1.reflection(posLight);
		float3 R2 = n2.reflection(posLight);
		float3 R3 = n3.reflection(posLight);
		//float3 R1 = posLight - (n1 * n1.dot(posLight) * 2.0f);
		//float3 R2 = posLight - (n2 * n2.dot(posLight) * 2.0f);
		//float3 R3 = posLight - (n3 * n3.dot(posLight) * 2.0f);
		//float3 R1 = posLight.reflection(n1);
		//float3 R2 = posLight.reflection(n2);
		//float3 R3 = posLight.reflection(n3);
		float diffuse1 = posLight.dot(n1);
		float diffuse2 = posLight.dot(n2);
		float diffuse3 = posLight.dot(n3);
		float tmp1 = R1.dot(v1);
		float tmp2 = R2.dot(v2);
		float tmp3 = R3.dot(v3);
		if (tmp1 < 0)
			tmp1 = 0;
		if (tmp2 < 0)
			tmp2 = 0;
		if (tmp3 < 0)
			tmp3 = 0;
		float specular1 = 0;
		float specular2 = 0;
		float specular3 = 0;

		specular1 = pow(tmp1, light->shininess);
		specular2 = pow(tmp2, light->shininess);
		specular3 = pow(tmp3, light->shininess);
		float3 col1; //= float3(0.5f, 0.0f, 0.0f);
		float3 col2; //= float3(0.0f, 0.5f, 0.0f);
		float3 col3; //= float3(0.0f, 0.0f, 0.5f);
		if (specular1 > 1)
			specular1 = 1;
		if (specular2 > 1)
			specular2 = 1;
		if (specular3 > 1)
			specular3 = 1;
		if (specular1 < 0)
			specular1 = 0;
		if (specular2 < 0)
			specular2 = 0;
		if (specular3 < 0)
			specular3 = 0;
		col1 = light->ambient + light->diffuse * diffuse1 + light->specular * specular1;
		col2 = light->ambient + light->diffuse * diffuse2 + light->specular * specular2;
		col3 = light->ambient + light->diffuse * diffuse3 + light->specular * specular3;
		//col1 = light->diffuse * diffuse1 + light->specular * specular1;
		//col2 = light->diffuse * diffuse2 + light->specular * specular2;
		//col3 = light->diffuse * diffuse3 + light->specular * specular3;
		//col1 = light->ambient + light->diffuse * diffuse1;
		//col2 = light->ambient + light->diffuse * diffuse2;
		//col3 = light->ambient + light->diffuse * diffuse3;
		//col1 = light->specular * specular1;
		//col2 = light->specular * specular2;
		//col3 = light->specular * specular3;
		//col1 = light->diffuse * diffuse1;
		//col2 = light->diffuse * diffuse2;
		//col3 = light->diffuse * diffuse3;
		col1.ZeroOne();
		col2.ZeroOne();
		col3.ZeroOne();

		buffer.setBordersAndCons(&ver3W, &ver2W, &ver1W);
		buffer.culling();
		triangulate(&ver3W, &ver2W, &ver1W, &col3, &col2, &col1);
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

Rasterizer::Rasterizer(std::vector<Object>* objects, DirectionalLight* light)
{
	buffer.setSize(HEIGHT, WIDTH);

	this->objects = *objects;
	this->light = light;
	setUpVerticlesOfObjects();
}

void Rasterizer::triangulate(float3* ver1W, float3* ver2W, float3* ver3W, float3* col1, float3* col2, float3* col3)
{
	for (int y = buffer.miny; y <= buffer.maxy; y++)
	{
		for (int x = buffer.minx; x <= buffer.maxx; x++)
		{
			//if (((ver1W->x - ver2W->x) * (y - ver1W->y) - (ver1W->y - ver2W->y) * (x - ver1W->x)) > 0 &&
			//	((ver2W->x - ver3W->x) * (y - ver2W->y) - (ver2W->y - ver3W->y) * (x - ver2W->x)) > 0 &&
			//	((ver3W->x - ver1W->x) * (y - ver3W->y) - (ver3W->y - ver1W->y) * (x - ver3W->x)) > 0)
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
		for (int j = start; j < end; j++)
		{
			vertices.at(j) = objects.at(i).orginalVertices.at(j - start);
			vertices.at(j) = objects.at(i).verProc.obj2world * vertices.at(j);
			vertices.at(j) = objects.at(i).verProc.world2view * vertices.at(j);
			vertices.at(j) = objects.at(i).verProc.view2proj * vertices.at(j);
			float w = vertices.at(j).w;
			vertices.at(j) = float4(vertices.at(j).x / w, vertices.at(j).y / w, vertices.at(j).z / w, w);

			normals.at(j) = objects.at(i).normals.at(j - start);
			normals.at(j) = objects.at(i).verProc.obj2world * normals.at(j);
			normals.at(j) = objects.at(i).verProc.world2view * normals.at(j);
			normals.at(j) = objects.at(i).verProc.view2proj * normals.at(j);
			w = normals.at(j).w;
			normals.at(j) = float4(normals.at(j).x / w, normals.at(j).y / w, normals.at(j).z / w, w);

			verticesMinus.at(j) = -(objects.at(i).orginalVertices.at(j - start)); 
			verticesMinus.at(j) = objects.at(i).verProc.obj2world * verticesMinus.at(j);
			verticesMinus.at(j) = objects.at(i).verProc.world2view * verticesMinus.at(j);
			verticesMinus.at(j) = objects.at(i).verProc.view2proj * verticesMinus.at(j);
			w = verticesMinus.at(j).w;
			verticesMinus.at(j) = float4(verticesMinus.at(j).x / w, verticesMinus.at(j).y / w, verticesMinus.at(j).z / w, w);
		}
		light->position = float3(0, 1, 1);
		//light->position = objects.at(0).verProc.obj2world * light->position;
		light->position = objects.at(0).verProc.world2view * light->position;
		light->position = objects.at(0).verProc.view2proj * light->position;
		float w = light->position.w;
		light->position = float4(light->position.x / w, light->position.y / w, light->position.z / w, w);
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
			verticesMinus.push_back(-float4(obj->orginalVertices.at(j)));
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

//Rasterizer::Rasterizer()
//{
//	buffer.setSize(HEIGHT, WIDTH);
//	buffer.clearColor(100, 100, 100);
//	buffer.clearDepth();
//
//	//vertices.push_back(float4(0.86f, 1.1f, 0.3f));
//	//vertices.push_back(float4(0.0f, 1.6f, 2.3f));
//	//vertices.push_back(float4(-0.86f, 1.1f, 0.3f));
//	//vertices.push_back(float4(0.0f, 2.6f, 0.3f));
//
//	//triangleIndexes.push_back(2);
//	//triangleIndexes.push_back(1);
//	//triangleIndexes.push_back(0);
//
//	//triangleIndexes.push_back(1);
//	//triangleIndexes.push_back(3);
//	//triangleIndexes.push_back(0);
//
//	//triangleIndexes.push_back(1);
//	//triangleIndexes.push_back(2);
//	//triangleIndexes.push_back(3);
//
//	//triangleIndexes.push_back(0);
//	//triangleIndexes.push_back(3);
//	//triangleIndexes.push_back(2);
//
//	vertices.push_back(float4());
//	vertices.push_back(float4());
//	vertices.push_back(float4());
//	vertices.push_back(float4());
//	orginalVertices.push_back(float4(0.86f, -0.5f, 0.0f));
//	orginalVertices.push_back(float4(-0.86f, -0.5f, 0.0f));
//	orginalVertices.push_back(float4(0.0f, 0.0f, 1.0f));
//	orginalVertices.push_back(float4(0.0f, 1.0f, 0.0f));
//
//	triangleIndexes.push_back(2);
//	triangleIndexes.push_back(1);
//	triangleIndexes.push_back(0);
//
//	triangleIndexes.push_back(1);
//	triangleIndexes.push_back(3);
//	triangleIndexes.push_back(0);
//
//	triangleIndexes.push_back(1);
//	triangleIndexes.push_back(2);
//	triangleIndexes.push_back(3);
//
//	triangleIndexes.push_back(0);
//	triangleIndexes.push_back(3);
//	triangleIndexes.push_back(2);
//
//	//vertices.push_back(float3(-0.5f, -0.5f, 0.8f));
//	//vertices.push_back(float3(-0.5f, 0.5f, 0.8f));
//	//vertices.push_back(float3(0.4f, 0.0f, 0.0f));
//	//vertices.push_back(float3(-0.5f, -0.5f, 0.3f));
//	//vertices.push_back(float3(-0.5f, 0.5f, 0.3f));
//	//vertices.push_back(float3(0.5f, 0.5f, 0.3f));
//	//triangleIndexes.push_back(0);
//	//triangleIndexes.push_back(1);
//	//triangleIndexes.push_back(2);
//
//	//vertices.push_back(float3(-0.5f, -0.5f, 0.3f));
//	//vertices.push_back(float3(0.5f, -0.5f, 0.3f));
//	//vertices.push_back(float3(0.5f, 0.5f, 0.3f));
//	//triangleIndexes.push_back(5);
//	//triangleIndexes.push_back(4);
//	//triangleIndexes.push_back(3);
//}
