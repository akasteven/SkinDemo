#pragma once

#include <D3DX11.h>
#include <xnamath.h>
#include "MathUtility.h"
#include "Vertex.h"

class AABB
{
public:

	AABB() :Center(0.0f, 0.0f, 0.0f), Extents(0.0f, 0.0f, 0.0f), Mins(0.0f, 0.0f, 0.0f), Maxs(0.0f, 0.0f, 0.0f){}
	~AABB();

	void Clear(){ Mins = XMFLOAT3(0.0f, 0.0f, 0.0f); Maxs = XMFLOAT3(0.0f, 0.0f, 0.0f); }
	void BuildFromVertices(Vertex::VertexBase * vertex, int numVer);

private:

	void ComputeCenterExt();

public:

	XMFLOAT3 Center;
	XMFLOAT3 Extents;
	XMFLOAT3 Mins;
	XMFLOAT3 Maxs;
};
