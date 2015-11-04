#pragma once
#include <d3dx9.h>
#include "d3dUtility.h"
#define M_RADIUS 0.21   // ball radius
#define PI 3.14159265
#define M_HEIGHT 0.01
#define DECREASE_RATE 0.9982
class CSphere;
class CObject{
public:
	CObject();
	virtual ~CObject();
	virtual bool hasIntersected(CSphere& object)=0;
	virtual void hitBy(CSphere& object) = 0;
	virtual void draw(IDirect3DDevice9* pDevice,
					   const D3DXMATRIX& mWorld,
					   const D3DXMATRIX& mView,
					   const D3DXMATRIX& mproj) = 0;
	virtual void destroy() = 0;
	void inline setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }
	virtual void setPosition(float x, float y, float z) = 0;
	const inline D3DXMATRIX&  getLocalTransform(void) const { return m_mLocal; }
protected:
	bool create(IDirect3DDevice9* pDevice, D3DXCOLOR color = d3d::WHITE);
	float m_x;
	float m_y;
	float m_z;
	D3DMATERIAL9            m_mtrl;
	D3DXMATRIX              m_mLocal;
	ID3DXMesh*              m_pMesh;
};