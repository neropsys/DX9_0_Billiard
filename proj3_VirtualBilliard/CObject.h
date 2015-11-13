#pragma once
#include <d3dx9.h>
#include "d3dUtility.h"
#include "ConstVariable.h"
class CObject{
public:
	enum Shape{
		SPHERE,
		CYLINDER,
		BOX
	};
	CObject();
	virtual ~CObject();

	virtual void draw(IDirect3DDevice9* pDevice,
					   const D3DXMATRIX& mWorld,
					   const D3DXMATRIX& mView) = 0;
	void destroy();
	void inline setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }
	void setPosition(float x, float y, float z);
	const inline D3DXMATRIX&  getLocalTransform(void) const { return m_mLocal; }
protected:
#define FVF_VERTEX D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1
	struct VERTEX
	{
		D3DXVECTOR3 pos;     // vertex position
		D3DXVECTOR3 norm;    // vertex normal
		float tu;            // texture coordinates
		float tv;
	};
	D3DVERTEXELEMENT9 decl[4] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, sizeof(D3DXVECTOR3), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, sizeof(D3DXVECTOR3)* 2, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	bool create(IDirect3DDevice9* pDevice, CObject::Shape shape);
	float m_x;
	float m_y;
	float m_z;
	//m_effect와 m_shaderCode, m_texture로 대체될 예정, 
	LPD3DXEFFECT			m_effect;
	D3DXMATRIX              m_mLocal;
	LPD3DXMESH              m_pMesh;
	LPD3DXBUFFER			m_shaderCode;
	LPDIRECT3DTEXTURE9		m_texture;
};