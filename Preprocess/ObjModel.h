#pragma once

#include <d3dx11.h>
#include <xnamath.h>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include "Vertex.h"

class ObjModel
{
public:
	ObjModel();
	~ObjModel();

	static void PreReadObjModel(
		const std::string filename,
		const std::string outname,
		bool bComputeNormals = false,
		bool bAverageNormals = false,
		bool bComputeTangents = false,
		bool bAverageTangents = false
		);
};

