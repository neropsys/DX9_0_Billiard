#include "CCue.h"
#include "ConstVariable.h"
CCue::CCue(){}
CCue::~CCue(){}
bool CCue::create(IDirect3DDevice9* pDevice, CObject::Shape shape){
	HRESULT result;
	char* effectFile;
	char* textureFile;
	switch (shape){
	case SPHERE:
		effectFile = SPHERE_EFFECT;
		textureFile = SPHERE_TEXTURE;
		result = D3DXCreateSphere(pDevice, M_RADIUS, SPHERE_SLICE, SPHERE_STACK, &m_pMesh, NULL));
		break;
	case CYLINDER:
		result = D3DXCreateCylinder(pDevice, 10, 10, 50, 50, 50, &m_pMesh, NULL);
		break;
	case BOX:
		break;
	}
	if (FAILED(result)){
		return false;
	}
	
	m_pMesh = convertMesh(pDevice, m_pMesh);
	if (m_pMesh == nullptr) return false;

	m_effect = LoadShader(pDevice, CUE_EFFECT);
	if (m_effect == nullptr) return false;
	
	m_texture = LoadTexture(pDevice, CUE_TEXTURE);
	if (m_texture == nullptr) return false;

	return true;
	//위 코드는 object::create로 옮길 예정
}
void CCue::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld, const D3DXMATRIX& mView){
	if (NULL == pDevice)
		return;
	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
	m_pMesh->DrawSubset(0);
}
LPD3DXMESH CCue::convertMesh(IDirect3DDevice9* pDevice, const LPD3DXMESH& input){

	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, sizeof(D3DXVECTOR3), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, sizeof(D3DXVECTOR3)* 2, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()//will be declared on 
	};

	LPD3DXMESH newMesh = nullptr;
	VERTEX* pVerts;
	HRESULT result = input->CloneMesh(D3DXMESH_SYSTEMMEM, decl, pDevice, &newMesh);
	
	if (FAILED(result)) return nullptr;
	
	if (SUCCEEDED(newMesh->LockVertexBuffer(0, (LPVOID*)&pVerts))){
		int numVerts = newMesh->GetNumVertices();
		for (int i = 0; i < numVerts; i++){
			D3DXVECTOR3 v = pVerts->pos - getPosition();
			D3DXVec3Normalize(&v, &v);
			pVerts->tu = asin(v.x) / D3DX_PI + .5f;
			pVerts->tv = asin(v.y) / D3DX_PI + .5f;
			pVerts++;
		}
		newMesh->UnlockVertexBuffer();
		//temporary uv generator
		return newMesh;
	}
	else{	
		return nullptr;
	}

}

LPD3DXEFFECT CCue::LoadShader(IDirect3DDevice9* pDevice, const char* fileName){
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
		if (ack) {
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
			//d3d::Release<LPD3DXBUFFER>(pError);
		}
	}
	return ret;
}
