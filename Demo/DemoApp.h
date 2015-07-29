#pragma once

#include "DemoBase.h"

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

	void BuidGeometry();
	void CreateShaders();
	void CreateContantBuffers();
	void CreateRenderStates();
	void CreateSamplerStates();
	void SetUpMatrices();

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

	XMMATRIX m_World;
	XMMATRIX m_View;
	XMMATRIX m_Proj;


	POINT mLastMousePos;
	float mTheta;
	float mPhi;
	float mRadius;
};

