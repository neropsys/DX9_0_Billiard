#pragma once
#include "CObject.h"
#include "d3dUtility.h"
#define SPHERE_VS_NAME "SphereVS.hlsl"
#define SPHERE_PS_NAME "SpherePS.hlsl"
#define SPHERE_TEXTURE "wood.jpg"
class CSphere : public CObject{
public:
	CSphere();
	~CSphere();

	bool create(IDirect3DDevice9* pDevice, D3DXCOLOR color = d3d::WHITE);
	void destroy(void) override;

	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld,
		const D3DXMATRIX& mView,
		const D3DXMATRIX& mProj) override;

	bool hasIntersected(CSphere& ball) override;
	void hitBy(CSphere& ball) override;

	void ballUpdate(float timeDiff);

	double inline getVelocity_X() { return this->m_velocity_x; }
	double inline getVelocity_Z() { return this->m_velocity_z; }
	float inline getRadius(void)  const { return (float)(M_RADIUS); }
	const inline D3DXVECTOR3 getCenter(void) const
	{
		D3DXVECTOR3 org(m_x, m_y, m_z);
		return org;
	}

	void setCenter(float x, float y, float z);
	void inline setPower(double vx, double vz)
	{
		this->m_velocity_x = vx;
		this->m_velocity_z = vz;
	}

private:
	float                   m_radius;
	float					m_velocity_x;
	float					m_velocity_z;
	LPD3DXEFFECT			m_effect;
	LPDIRECT3DTEXTURE9		m_texture;
	LPD3DXEFFECT LoadShader(IDirect3DDevice9* pDevice, const char* fileName);
	LPDIRECT3DTEXTURE9 LoadTexture(IDirect3DDevice9* pDevice, const char* fileName);
	void setPosition(float x, float y, float z) override;
};