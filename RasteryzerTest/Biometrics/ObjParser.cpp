#include "ObjParser.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "float3.h"

void ObjParser::readMeshFromFile(const std::string& filename, Object* object)
{
	std::ifstream input;
	std::regex vertex("(v )([-]?\\d*\\.\\d*)\\s([-]?\\d*\\.\\d*)\\s([-]?\\d*\\.\\d*)");
	std::regex normal("(vn )([-]?\\d*\\.\\d*)\\s([-]?\\d*\\.\\d*)\\s([-]?\\d*\\.\\d*)");
	std::regex face("(f )(\\d*)\/{2}(\\d*)\\s(\\d*)\/{2}(\\d*)\\s(\\d*)\/{2}(\\d*)");
	try
	{
		std::string line;
		input.open(filename);
		while (!input.eof())
		{
			std::getline(input, line);
			std::smatch result;
			if (std::regex_search(line, result, vertex))
				object->orginalVertices.push_back(float3(
					std::stof(result[2].str().c_str()),
					std::stof(result[3].str().c_str()),
					std::stof(result[4].str().c_str())));
			//if (std::regex_search(line, result, normal))
			//	object->normals.push_back(float3(
			//		std::stof(result[2].str().c_str()),
			//		std::stof(result[3].str().c_str()),
			//		std::stof(result[4].str().c_str())));
			if (std::regex_search(line, result, face))
			{
				//object->triangleIndexes.push_back((std::stoi(result[2]) - 1));
				//object->triangleIndexes.push_back((std::stoi(result[4]) - 1));
				//object->triangleIndexes.push_back((std::stoi(result[6]) - 1));

				object->triangleIndexes.push_back((std::stoi(result[6]) - 1));
				object->triangleIndexes.push_back((std::stoi(result[4]) - 1));
				object->triangleIndexes.push_back((std::stoi(result[2]) - 1));
			}
		}
	}
	catch (const std::ifstream::failure & e)
	{
		std::cerr << "Could not open file!!! \nError: " << e.what() << "\n";
	}
	catch (const std::exception & e)
	{
		std::cerr << "Unknown error: " << e.what() << std::endl;
	}

	input.close();
}
