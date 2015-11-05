#pragma once
#include "CObject.h"
#include "d3dUtility.h"
class CWall : public CObject{
public:
	CWall();
	~CWall();

	bool create(IDirect3DDevice9* pDevice,
				float ix,
				float iz,
				float iwidth,
				float iheight,
				float idepth,
				D3DXCOLOR color = d3d::WHITE);
	void destroy() override;

	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld,
		const D3DXMATRIX& mView,
		const D3DXMATRIX& mProj) override;

	bool hasIntersected(CSphere& ball) override;
	void hitBy(CSphere& ball) override;

	void setPosition(float x, float y, float z) override;
	const D3DXVECTOR3 inline getPosition(){ return D3DXVECTOR3(m_x, m_y, m_z); }
	inline float getHeight() const { return M_HEIGHT; }

private:
	using CObject::setLocalTransform;

	float                   m_width;
	float                   m_depth;
	float					m_height;


};