#pragma once

#include "DemoBase.h"
#include "ShadowMap.h"

struct BoundingSphere
{
	BoundingSphere() :center(0.0f, 0.0f, 0.0f), radius(0.0f){}
	XMFLOAT3 center;
	float radius;
};

class DemoApp :
	public DemoBase
{
public:
	DemoApp(HINSTANCE hInstance);
	~DemoApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:

	//Common setups
	void CreateGeometry();
	void CreateShaders();
	void CreateContantBuffers();
	void CreateRenderStates();
	void CreateSamplerStates();
	void SetUpMatrices();

	//Shadow Map
	void RenderShadowMap();


private:

	ID3D11VertexShader * m_pVertexShader;
	ID3D11PixelShader * m_pPixelShader;

	ID3D11Buffer * m_pCBNeverChanges;
	ID3D11Buffer * m_pCBOnResize;
	ID3D11Buffer * m_pCBPerFrame;
	ID3D11Buffer * m_pCBPerObject;

	ID3D11Buffer * m_pVertexBuffer;
	ID3D11Buffer * m_pIndexBuffer;
	UINT numVertex;
	UINT numTriangle;

	ID3D11ShaderResourceView * m_pTextureSRV;
	ID3D11SamplerState * m_pSampleLinear;
	ID3D11ShaderResourceView * m_pNormalMapSRV;

	ShadowMap *m_pShadowMap;
	int  mShadowMapSize = 4096;

	XMMATRIX m_World;
	XMMATRIX m_View;
	XMMATRIX m_Proj;

	POINT mLastMousePos;
	float mTheta;
	float mPhi;
	float mRadius;
};

