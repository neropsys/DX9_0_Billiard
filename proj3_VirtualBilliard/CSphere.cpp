#include "CSphere.h"
#include <d3d9.h>
CSphere::CSphere(){
	D3DXMatrixIdentity(&m_mLocal);
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
	m_radius = 0;
	m_velocity_x = 0;
	m_velocity_z = 0;
	m_pMesh = NULL;
}
CSphere::~CSphere(){}
bool CSphere::create(IDirect3DDevice9* pDevice, D3DXCOLOR color){
	//if (!CObject::create(pDevice, color))
	//return false;
	
	m_texture = LoadTexture(pDevice, SPHERE_TEXTURE);
	if (!m_texture)
		return false;
		
	m_effect = LoadShader(pDevice, SPHERE_EFFECT);
	if (!m_effect)
		return false;
	m_pMesh = createMesh(pDevice, getRadius(), 50, 50);

	return true;
}
void CSphere::destroy(){

	if (m_pMesh != NULL) {
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	if (m_shaderCode != NULL){
		m_shaderCode->Release();
		m_shaderCode = NULL;
	}
	if (m_texture != NULL){
		m_texture->Release();
		m_texture = NULL;
	}
	if (m_effect != NULL){
		m_effect->Release();
		m_effect = NULL;
	}
	if (m_adj != NULL){
		m_adj->Release();
		m_adj = NULL;
	}


}
void CSphere::draw(IDirect3DDevice9* pDevice,
	const D3DXMATRIX& mWorld,
	const D3DXMATRIX& mView,
	const D3DXMATRIX& mProj)
{
	if (NULL == pDevice)
		return;
	
	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
	m_effect->SetMatrix("gWorldMatrix", &m_mLocal);
	m_effect->SetMatrix("gViewMatrix", &mView);
	m_effect->SetMatrix("gProjectionMatrix", &mProj);
	m_effect->SetTexture("DiffuseMap", m_texture);
	//m_effect->

	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_effect->SetTechnique("ColorShader");
	UINT numPass = 0;
	m_effect->Begin(&numPass, NULL);
	{
		for (UINT i = 0; i < numPass; ++i)
		{
			m_effect->BeginPass(i);
		
			m_pMesh->DrawSubset(0);
			m_effect->EndPass();
		}
	}
	m_effect->End();

	//pDevice->SetMaterial(&m_mtrl);
	/*
	pDevice->SetTexture(0, m_texture);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	m_pMesh->DrawSubset(0);
	
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	m_effect->SetMatrix("gWorld", &mWorld);
	m_effect->SetMatrix("gView", &mView);
	m_effect->SetMatrix("gProj", &mProj);
	m_effect->SetTexture("DiffuseMap", m_texture);
	//pDevice->SetTexture(0, m_texture);
	UINT numPass = 0;
	D3DXHANDLE hTech = m_effect->GetTechniqueByName("SphereVS");
	m_effect->SetTechnique(hTech);
	
	m_effect->Begin(&numPass, NULL);
	{
		for (UINT i = 0; i < numPass; ++i)
		{
			m_effect->BeginPass(i);
			{
				m_pMesh->DrawSubset(0);
			}
			m_effect->EndPass();
		}
	}
	m_effect->End();
	*/

}
bool CSphere::hasIntersected(CSphere& ball)
{
	// Insert your code here.
	return false;
}
void CSphere::hitBy(CSphere& ball)
{
	// Insert your code here.
}
void CSphere::ballUpdate(float timeDiff)
{
	const float TIME_SCALE = 3.3;
	D3DXVECTOR3 cord = this->getCenter();
	double vx = abs(this->getVelocity_X());
	double vz = abs(this->getVelocity_Z());

	if (vx > 0.01 || vz > 0.01)
	{
		float tX = cord.x + TIME_SCALE*timeDiff*m_velocity_x;
		float tZ = cord.z + TIME_SCALE*timeDiff*m_velocity_z;

		//correction of position of ball
		// Please uncomment this part because this correction of ball position is necessary when a ball collides with a wall
		if (tX >= (4.5 - M_RADIUS))
			tX = 4.5 - M_RADIUS;
		else if (tX <= (-4.5 + M_RADIUS))
			tX = -4.5 + M_RADIUS;
		else if (tZ <= (-3 + M_RADIUS))
			tZ = -3 + M_RADIUS;
		else if (tZ >= (3 - M_RADIUS))
			tZ = 3 - M_RADIUS;

		this->setCenter(tX, cord.y, tZ);
	}
	else { this->setPower(0, 0); }
	//this->setPower(this->getVelocity_X() * DECREASE_RATE, this->getVelocity_Z() * DECREASE_RATE);
	double rate = 1 - (1 - DECREASE_RATE)*timeDiff * 400;
	if (rate < 0)
		rate = 0;
	this->setPower(getVelocity_X() * rate, getVelocity_Z() * rate);
}
void CSphere::setCenter(float x, float y, float z){
	setPosition(x, y, z);
}
void CSphere::setPosition(float x, float y, float z){
	D3DXMATRIX m;
	m_x = x;	m_y = y;	m_z = z;
	D3DXMatrixTranslation(&m, x, y, z);
	setLocalTransform(m);
}
LPD3DXEFFECT CSphere::LoadShader(IDirect3DDevice9* pDevice, const char* fileName){
	LPD3DXBUFFER pError = NULL;
	LPD3DXEFFECT ret = NULL;
	DWORD dwShaderFlags = 0;
#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif
	D3DXCreateEffectFromFile(pDevice, fileName, 0, 0, dwShaderFlags, 0, &ret, &pError);
	if (!ret && pError){
		int size = pError->GetBufferSize();
		void* ack = pError->GetBufferPointer();
		if (ack){
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
			//d3d::Release<LPD3DXBUFFER>(pError);
		}
	}

	/*
	D3DXCompileShaderFromFile(
		SPHERE_VS_NAME,
		0,
		0,
		"main",
		"vs_3_0",
		dwShaderFlags,
		&ret,
		&pError,
		&m_constTable);
	if (!ret && pError){
		int size = pError->GetBufferSize();
		void* ack = pError->GetBufferPointer();
		if (ack){
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
			//d3d::Release<LPD3DXBUFFER>(pError);
		}
	}
	if (FAILED(pDevice->CreateVertexShader((DWORD*)ret->GetBufferPointer(), &m_shader))){
		OutputDebugString("Failed to create vertex shader");
		return NULL;
	}
	
	//D3DXCompileShaderFromFile(fileName, 0, 0, "main", "vs_3_0", dwShaderFlags, &m_effect, &pError, NULL);
	D3DXCreateEffectFromFile(pDevice, fileName, NULL, NULL, dwShaderFlags, NULL, &ret, &pError);
	if (!ret && pError){
		int size = pError->GetBufferSize();
		void* ack = pError->GetBufferPointer();
		if (ack){
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
		}
	}*/

	return ret;

}
LPDIRECT3DTEXTURE9 CSphere::LoadTexture(IDirect3DDevice9* pDevice, const char* fileName){
	LPDIRECT3DTEXTURE9 ret = NULL;
	if (FAILED(D3DXCreateTextureFromFile(pDevice, fileName, &ret))){
		OutputDebugString("Failed to load texture: ");
		OutputDebugString(fileName);
		OutputDebugString("\n");
	}
	return ret;
}
LPD3DXMESH CSphere::createMesh(IDirect3DDevice9* pDevice, float rad, UINT slices, UINT stacks){
	LPD3DXMESH mesh;
	if (FAILED(D3DXCreateSphere(pDevice, rad, slices, stacks, &mesh, NULL))){
		return NULL;
	}

	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, sizeof(D3DXVECTOR3), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, sizeof(D3DXVECTOR3) * 2, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	//IDirect3DVertexDeclaration9* ppDecl;
	//DWORD numFaces = mesh->GetNumFaces();
	//DWORD numVertices = mesh->GetNumVertices();
	//DWORD options = mesh->GetOptions();

	LPD3DXMESH newMesh;
	mesh->CloneMesh(D3DXMESH_SYSTEMMEM, decl, pDevice, &newMesh);


	//if (FAILED(mesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_VERTEX, pDevice, &newMesh))){
	//	return mesh;
	//}
	//if (FAILED(D3DXCreateMesh(numFaces, numVertices, options, decl, pDevice, &newMesh))){
	//	return mesh;
	//}
	//if (FAILED(pDevice->CreateVertexDeclaration(decl, &ppDecl))){
	//	return mesh;
//	}
	VERTEX* pVerts;
	if (SUCCEEDED(newMesh->LockVertexBuffer(0, (LPVOID*)&pVerts))){
		int numVerts = newMesh->GetNumVertices();
		for (int i = 0; i < numVerts; i++){
			pVerts->tu = asin(pVerts->norm.x) / D3DX_PI + .5f;
			pVerts->tv = asin(pVerts->norm.y) / D3DX_PI + .5f;
			pVerts++;
		}
		newMesh->UnlockVertexBuffer();
	}

	/*
	if (SUCCEEDED(newMesh->LockVertexBuffer(0, (LPVOID*)&pVerts))){
		int numVerts = newMesh->GetNumVertices();
		for (int i = 0; i < numVerts; i++){
			pVerts->tu = asin(pVerts->norm.x) / D3DX_PI + .5f;
			pVerts->tv = asin(pVerts->norm.y) / D3DX_PI + .5f;
			pVerts++;
		}
		newMesh->UnlockVertexBuffer();
	}*/
	mesh->Release();
	return newMesh;

}