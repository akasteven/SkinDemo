//***************************************************************************************
// Vertex.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Defines vertex structures and input layouts.
//***************************************************************************************

#pragma once
#include <d3dx11.h>
#include <xnamath.h>
#include "D3DUtil.h"

namespace Vertex
{
	enum VERTEXFORMAT
	{
		POS = 1,
		POSCOL = 2,
		POSNOR = 3,
		POSTEX = 4,
		POSNORTEX = 5,
		POSNORTEXTAN = 6,
	};

	struct VertexPT
	{
		VertexPT() : Pos(0.0f, 0.0f, 0.0f), Tex(0.0f, 0.0f) {}
		VertexPT(const XMFLOAT3& p, const XMFLOAT2& uv)
			: Pos(p), Tex(uv) {}
		VertexPT(float px, float py, float pz, float nx, float ny, float nz, float u, float v)
			: Pos(px, py, pz), Tex(u, v) {}
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
	};

	struct VertexPNT
	{
		VertexPNT() : Pos(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 0.0f), Tex(0.0f, 0.0f) {}
		VertexPNT(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT2& uv)
			: Pos(p), Normal(n), Tex(uv) {}
		VertexPNT(float px, float py, float pz, float nx, float ny, float nz, float u, float v)
			: Pos(px, py, pz), Normal(nx, ny, nz), Tex(u,v) {}
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
	};

	struct  VertexPNTTan
	{
		VertexPNTTan() : Pos(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 0.0f), Tex(0.0f, 0.0f), Tan(0.0f, 0.0f, 0.0f) {}
		VertexPNTTan(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT2& uv, const XMFLOAT3& tan)
			: Pos(p), Normal(n), Tex(uv), Tan(tan) {}
		VertexPNTTan(float px, float py, float pz, float nx, float ny, float nz, float u, float v, float tx, float ty, float tz)
			: Pos(px, py, pz), Normal(nx, ny, nz), Tex(u, v), Tan(tx, ty, tz) {}

		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
		XMFLOAT3 Tan;
	};
}



class InputLayoutDesc
{
public:
	// Init like const int A::a[4] = {0, 1, 2, 3}; in .cpp file.
	static const D3D11_INPUT_ELEMENT_DESC VertexPT[2];
	static const D3D11_INPUT_ELEMENT_DESC VertexPNT[3];
	static const D3D11_INPUT_ELEMENT_DESC VertexPNTTan[4];
};

class InputLayouts
{
public:
	static void InitLayout(ID3D11Device* device, ID3DBlob * blob, Vertex::VERTEXFORMAT format);
	static void DestroyAll();

	static ID3D11InputLayout* VertexPT;     
	static ID3D11InputLayout* VertexPNT;
	static ID3D11InputLayout * VertexPNTTan;
};

