#pragma once
#include <string>
#include <regex>
#include "Object.h"

class ObjParser
{
public:
	void static readMeshFromFile(const std::string& filename, Object* object);
};

