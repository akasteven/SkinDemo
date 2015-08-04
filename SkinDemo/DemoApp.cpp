#include "DemoApp.h"
#include "Vertex.h"
#include "RenderStates.h"
#include "ShadowMap.h"
#include "AABB.h"

struct CBNeverChanges
{
	DirectionalLight dirLight;
	PointLight pointLight;
	float shadowMapSize;
	XMFLOAT3 pad3;
};

struct CBOnResize
{
	XMMATRIX mProjection;
};

struct CBPerFrame
{
	XMFLOAT3 eyePos;
	float pad;
	XMMATRIX matLightVPT;
};

struct CBPerObject
{
	XMMATRIX matWorld;
	XMMATRIX matWorldInvTranspose;
	XMMATRIX matWVP;
	Material material;
};

struct CBPerObjectShadow
{
	XMMATRIX lightWVP;
};

DemoApp::DemoApp(HINSTANCE hInstance)
:DemoBase(hInstance),
m_pVertexShader(0),
m_pPixelShader(0),
m_pCBNeverChanges(0),
m_pCBOnResize(0),
m_pCBPerFrame(0),
m_pCBPerObject(0),
m_pCBPerObjShadow(0),
m_pVertexBuffer(0),
m_pIndexBuffer(0),
m_pTextureSRV(0),
m_pNormalMapSRV(0),
m_pDepthSRV(0),
m_pSampleLinear(0),
numVertex(0), 
mTheta(-0.5f*MathHelper::Pi), 
mPhi(0.5f*MathHelper::Pi), 
mRadius(40.0f),
m_pShadowMap(0),
mShadowMapSize(4096),
m_pAABB(0)
{
	this->mMainWndCaption = L"Demo";
	mLastMousePos.x = 0;
	mLastMousePos.y = 0;
}

DemoApp::~DemoApp()
{
	if (m_pShadowMap)
	{
		delete m_pShadowMap;
		m_pShadowMap = 0;
	}

	if (m_pAABB)
	{
		delete m_pAABB;
		m_pAABB = 0;
	}

	md3dImmediateContext->ClearState();
	ReleaseCOM(m_pVertexBuffer);
	ReleaseCOM(m_pIndexBuffer);
	ReleaseCOM(m_pCBNeverChanges);
	ReleaseCOM(m_pCBOnResize);
	ReleaseCOM(m_pCBPerFrame);
	ReleaseCOM(m_pCBPerObject);
	ReleaseCOM(m_pCBPerObjShadow);
	ReleaseCOM(m_pTextureSRV);
	ReleaseCOM(m_pNormalMapSRV);
	ReleaseCOM(m_pSampleLinear);
	ReleaseCOM(m_pVertexShader);
	ReleaseCOM(m_pPixelShader);
	ReleaseCOM(m_pShadowMapVS);
	ReleaseCOM(m_pShadowMapPS);

	InputLayouts::DestroyAll();
	RenderStates::DestroyAll();
}
 
void DemoApp::OnResize()
{
	DemoBase::OnResize();
	m_Proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, mClientWidth / (float)mClientHeight, 0.01f, 100.0f);
}

void DemoApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void DemoApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void DemoApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta -= dx;
		mPhi -= dy;

		// Restrict the angle mPhi.
		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to 0.01 unit in the scene.
		float dx = 0.1f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.1f*static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = MathHelper::Clamp(mRadius, 35.0f, 75.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void DemoApp::CreateLights()
{
	mDirLight.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mDirLight.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mDirLight.Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	mDirLight.Direction = XMFLOAT3(1.0f, 0.0f, 1.0f);

	mPointLight.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mPointLight.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	mPointLight.Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	mPointLight.Position = XMFLOAT3(80.0f, 0.0f, -80.0f);
	mPointLight.Range = 500.0f;
	mPointLight.Att = XMFLOAT3(1.0f, 0.2f, 0.0f);
}

void DemoApp::CreateShaders()
{
	//Default VS
	ID3DBlob *pVSBlob = NULL;
	HR(LoadShaderBinaryFromFile("Shaders//vs.fxo", &pVSBlob));
	HR(md3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader));
	InputLayouts::InitLayout(md3dDevice, pVSBlob, Vertex::POSNORTEXTAN);

	//Shadow map VS
	ID3DBlob *pShadowVSBlob = NULL;
	HR(LoadShaderBinaryFromFile("Shaders//shadowvs.fxo", &pShadowVSBlob));
	HR(md3dDevice->CreateVertexShader(pShadowVSBlob->GetBufferPointer(), pShadowVSBlob->GetBufferSize(), NULL, &m_pShadowMapVS));

	//Default PS
	ID3DBlob *pPSBlob = NULL;
	HR(LoadShaderBinaryFromFile("Shaders//ps.fxo", &pPSBlob));
	HR(md3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader));

	//Shadow map PS 
	ID3DBlob *pShadowPSBlob = NULL;
	HR(LoadShaderBinaryFromFile("Shaders//shadowps.fxo", &pShadowPSBlob));
	HR(md3dDevice->CreatePixelShader(pShadowPSBlob->GetBufferPointer(), pShadowPSBlob->GetBufferSize(), NULL, &m_pShadowMapPS));

	ReleaseCOM(pVSBlob);
	ReleaseCOM(pPSBlob);
	ReleaseCOM(pShadowVSBlob);
	ReleaseCOM(pShadowPSBlob);
}

void DemoApp::CreateGeometry()
{
	LoadModel("..//Resources//Perry.objx", &m_pVertexBuffer, &m_pIndexBuffer, md3dDevice, m_pAABB, numVertex, numTriangle);
}

void DemoApp::CreateContantBuffers()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	desc.ByteWidth = sizeof(CBNeverChanges);
	HR(md3dDevice->CreateBuffer(&desc, 0, &m_pCBNeverChanges));

	desc.ByteWidth = sizeof(CBOnResize);
	HR(md3dDevice->CreateBuffer(&desc, 0, &m_pCBOnResize));

	desc.ByteWidth = sizeof(CBPerFrame);
	HR(md3dDevice->CreateBuffer(&desc, 0, &m_pCBPerFrame));

	desc.ByteWidth = sizeof(CBPerObject);
	HR(md3dDevice->CreateBuffer(&desc, 0, &m_pCBPerObject));

	desc.ByteWidth = sizeof(CBPerObjectShadow);
	HR(md3dDevice->CreateBuffer(&desc, 0, &m_pCBPerObjShadow));
}

void DemoApp::CreateSamplerStates()
{
	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, L"..//Resources//diffuse.jpg", NULL, NULL, &m_pTextureSRV, NULL));
	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, L"..//Resources//normal.BMP", NULL, NULL, &m_pNormalMapSRV, NULL));

	D3D11_SAMPLER_DESC desc; 
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; 
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	HR(md3dDevice->CreateSamplerState(&desc, &m_pSampleLinear));

	
}

void DemoApp::SetUpSceneConsts()
{
	//Set Invariant Constant Buffer
	CBNeverChanges cbNeverChanges;
	cbNeverChanges.dirLight = mDirLight;
	cbNeverChanges.pointLight = mPointLight;
	cbNeverChanges.shadowMapSize = static_cast<float>(mShadowMapSize);
	md3dImmediateContext->UpdateSubresource(m_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);

	m_Proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, mClientWidth / (float)mClientHeight, 0.01f, 100.0f);

	BuildShadowMapMatrices();
}

void DemoApp::CreateRenderStates()
{
	RenderStates::InitAll(md3dDevice);
}

void DemoApp::BuildShadowMapMatrices()
{
	float aabbRadius = m_pAABB->GetRadius();
	XMVECTOR lightDir = XMLoadFloat3(&mDirLight.Direction);
	XMVECTOR lightPos = -2.0f * lightDir * aabbRadius;
	XMVECTOR targetPos = XMLoadFloat3(&m_pAABB->Center);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	mLightView = XMMatrixLookAtLH(lightPos, targetPos, up);

	// Transform bounding sphere to light space.
	XMFLOAT3 aabbCenterLightSpace;
	XMStoreFloat3(&aabbCenterLightSpace, XMVector3TransformCoord(targetPos, mLightView));

	//// Ortho frustum in light space encloses scene.
	float l = aabbCenterLightSpace.x - aabbRadius;
	float b = aabbCenterLightSpace.y - aabbRadius;
	float n = aabbCenterLightSpace.z - aabbRadius;
	float r = aabbCenterLightSpace.x + aabbRadius;
	float t = aabbCenterLightSpace.y + aabbRadius;
	float f = aabbCenterLightSpace.z + aabbRadius;
	mLightProj = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	mLightViewport = XMMATRIX(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	 mLightVPT = mLightView*mLightProj*mLightViewport;
}

void DemoApp::RenderShadowMap()
{
	//Set Buffers, Layout, Topology and Render States
	UINT stride = sizeof(Vertex::VertexPNTTan);
	UINT offset = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	md3dImmediateContext->IASetInputLayout(InputLayouts::VertexPNTTan);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	md3dImmediateContext->RSSetState(RenderStates::ShadowMapDepthRS);

	CBPerObjectShadow cbPerObjShadow;
	cbPerObjShadow.lightWVP = XMMatrixTranspose(m_World * mLightView * mLightProj);
	md3dImmediateContext->UpdateSubresource(m_pCBPerObjShadow, 0, NULL, &cbPerObjShadow, 0, 0);

	md3dImmediateContext->VSSetShader(m_pShadowMapVS, NULL, 0);
	md3dImmediateContext->VSSetConstantBuffers(3, 1, &m_pCBPerObjShadow);

	md3dImmediateContext->PSSetShader(m_pShadowMapPS, NULL, 0);
	md3dImmediateContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
	md3dImmediateContext->PSSetSamplers(0, 1, &m_pSampleLinear);

	md3dImmediateContext->Draw(numVertex, 0);
}

bool DemoApp::Init()
{
	if (!DemoBase::Init())
		return false;

	m_pShadowMap = new ShadowMap(md3dDevice, mShadowMapSize, mShadowMapSize);
	m_pAABB = new AABB();
	CreateLights();
	CreateShaders();
	CreateGeometry();
	CreateContantBuffers();
	CreateSamplerStates();
	CreateRenderStates();
	SetUpSceneConsts();

	return true;
}

void DemoApp::UpdateScene(float dt)
{
	float x = mRadius*sinf(mPhi)*cosf(mTheta);
	float z = mRadius*sinf(mPhi)*sinf(mTheta);
	float y = mRadius*cosf(mPhi);

	//Update Per Frame Constant Buffer
	CBPerFrame cbPerFrame;
	cbPerFrame.eyePos = XMFLOAT3(x, y, z);
	cbPerFrame.matLightVPT = XMMatrixTranspose(mLightVPT);
	md3dImmediateContext->UpdateSubresource(m_pCBPerFrame, 0, NULL, &cbPerFrame, 0, 0);

	float t = mTimer.TotalTime();
	XMMATRIX rot = XMMatrixRotationY(t * 0.3);
	m_World = rot;

	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_View = XMMatrixLookAtLH(pos, target, up);
}

void DemoApp::DrawScene()
{
	assert(md3dImmediateContext);
	assert(mSwapChain);

	//Render shadow map
	m_pShadowMap->BindShadowMapDSV(md3dImmediateContext);
	RenderShadowMap();
	m_pDepthSRV = m_pShadowMap->DepthShaderResourceView();

	//Restore render targets
	md3dImmediateContext->RSSetState(0);
	ID3D11RenderTargetView* renderTargets[1] = { mRenderTargetView };
	md3dImmediateContext->OMSetRenderTargets(1, renderTargets, mDepthStencilView);
	md3dImmediateContext->RSSetViewports(1, &mScreenViewport);

	//Clear Render Targets
	float clearColor[4] = { 199.0f / 255.0f, 197.0f / 255.0f, 206.0f / 255.0f, 1.0f };
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, clearColor);
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//Set Buffers, Layout, Topology and Render States
	UINT stride = sizeof(Vertex::VertexPNTTan);
	UINT offset = 0;
	md3dImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	md3dImmediateContext->IASetInputLayout(InputLayouts::VertexPNTTan);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	md3dImmediateContext->RSSetState(RenderStates::CullClockwiseRS);

	//Update Per Object Constant Buffer
	CBPerObject cbPerObj;
	cbPerObj.matWorld = XMMatrixTranspose(m_World);
	cbPerObj.matWorldInvTranspose = XMMatrixTranspose(MathHelper::InverseTranspose(m_World));
	cbPerObj.matWVP = XMMatrixTranspose(m_World * m_View * m_Proj);
	cbPerObj.material.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cbPerObj.material.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	cbPerObj.material.Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	cbPerObj.material.Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	md3dImmediateContext->UpdateSubresource(m_pCBPerObject, 0, NULL, &cbPerObj, 0, 0);

	//Set Shaders and Resources
	md3dImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	md3dImmediateContext->VSSetConstantBuffers(0, 1, &m_pCBNeverChanges);
	md3dImmediateContext->VSSetConstantBuffers(1, 1, &m_pCBOnResize);
	md3dImmediateContext->VSSetConstantBuffers(2, 1, &m_pCBPerFrame);
	md3dImmediateContext->VSSetConstantBuffers(3, 1, &m_pCBPerObject);

	md3dImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
	md3dImmediateContext->PSSetConstantBuffers(0, 1, &m_pCBNeverChanges);
	md3dImmediateContext->PSSetConstantBuffers(1, 1, &m_pCBOnResize);
	md3dImmediateContext->PSSetConstantBuffers(2, 1, &m_pCBPerFrame);
	md3dImmediateContext->PSSetConstantBuffers(3, 1, &m_pCBPerObject);

	md3dImmediateContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
	md3dImmediateContext->PSSetShaderResources(1, 1, &m_pNormalMapSRV);
	md3dImmediateContext->PSSetShaderResources(2, 1, &m_pDepthSRV);
	md3dImmediateContext->PSSetSamplers(0, 1, &m_pSampleLinear);

	md3dImmediateContext->Draw(numVertex, 0);
	HR(mSwapChain->Present(0, 0));
}


