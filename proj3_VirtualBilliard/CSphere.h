#pragma once
#include "CObject.h"
#include "d3dUtility.h"
#define SPHERE_EFFECT "SphereEffect.fx"
#define SPHERE_PS_NAME "SpherePS.hlsl"
#define SPHERE_TEXTURE "checkered.png"
#define SPHERE_RED "redSmiley.png"
#define SPHERE_BLUE "blueSmiley.png"
#define SPHERE_YELLOW "yellowSmiley.png"
#define SPHERE_WHITE "whiteSmiley.png"
class CSphere : public CObject{
public:
	CSphere();
	~CSphere();

	bool create(IDirect3DDevice9* pDevice, D3DXCOLOR color = d3d::WHITE);
	void destroy(void) override;

	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld,
		const D3DXMATRIX& mView) override;
	void tempdraw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld,
		const D3DXMATRIX& mView,
		const D3DXVECTOR4& mLightPos);
	bool hasIntersected(CSphere& ball) override;
	void hitBy(CSphere& ball) override;

	void ballUpdate(float timeDiff);

	//caution : getters and setters for pre_velocity_x, pre_velocity_z, isChanged are added
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
	bool isStop(){
		return (m_velocity_x == 0 && m_velocity_z == 0);
		//x축 속도와 z축 속도가 모두 0일 경우 true 반환
	}
	static inline bool IsAllStop(CSphere r1, CSphere r2, CSphere w, CSphere y){
		return r1.isStop() && r2.isStop() && w.isStop() && y.isStop();
		//4개의 공이 모두 정지해 있을 경우 true 반환
	}
private:
#define FVF_VERTEX D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1
	struct VERTEX
	{
		D3DXVECTOR3 pos;     // vertex position
		D3DXVECTOR3 norm;    // vertex normal
		float tu;            // texture coordinates
		float tv;
	};

	float                   m_radius;
	float					m_velocity_x;
	float					m_velocity_z;
	LPD3DXBUFFER			m_shaderCode;
	ID3DXEffect*			m_effect;
	LPDIRECT3DTEXTURE9		m_texture;
	D3DXHANDLE				worldhdl;
	D3DXHANDLE				viewdhdl;
	D3DXHANDLE				projhdl;

	LPD3DXEFFECT LoadShader(IDirect3DDevice9* pDevice, const char* fileName);
	LPDIRECT3DTEXTURE9 LoadTexture(IDirect3DDevice9* pDevice, const char* fileName);
	LPD3DXMESH createMesh(IDirect3DDevice9* pDevice, float rad, UINT slices, UINT stacks);
	void setPosition(float x, float y, float z) override;
};
