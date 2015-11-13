#pragma once
#include "CObject.h"
#include "d3dUtility.h"
#include "ICollidable.h"
#define SPHERE_EFFECT "SphereEffect.fx"
#define SPHERE_PS_NAME "SpherePS.hlsl"
#define SPHERE_TEXTURE "checkered.png"
#define SPHERE_RED "redSmiley.png"
#define SPHERE_BLUE "blueSmiley.png"
#define SPHERE_YELLOW "yellowSmiley.png"
#define SPHERE_WHITE "whiteSmiley.png"
class CSphere : public CObject, ICollidable{
public:
	CSphere();
	~CSphere();

	bool create(IDirect3DDevice9* pDevice, CObject::Shape shape);

	void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld,
		const D3DXMATRIX& mView) override;
	void tempdraw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld,
		const D3DXMATRIX& mView,
		const D3DXVECTOR4& mLightPos);
	bool hasIntersected(CSphere& ball) override;
	void hitBy(CSphere& ball) override;

	void ballUpdate(float timeDiff);

	//caution : getters and setters for pre_velocity_x, pre_velocity_z, isChanged are added
	D3DXVECTOR3 inline getVelocity() const { return this->velocity; }
	float inline getRadius(void)  const { return (float)(M_RADIUS); }
	const inline D3DXVECTOR3 getCenter(void) const
	{
		D3DXVECTOR3 org(m_x, m_y, m_z);
		return org;
	}

	void setCenter(float x, float y, float z);
	void inline setPower(double vx, double vz)
	{
		this->velocity.x = vx;
		this->velocity.z = vz;
	}

	float inline getPower(){
		return velocity.x * velocity.x + velocity.z * velocity.z;
	}

	void inline setPower(D3DXVECTOR3 velocity){
		this->velocity = velocity;
	}

	bool inline isStop(){
		return (velocity.x == 0 && velocity.z == 0);
		//x축 속도와 z축 속도가 모두 0일 경우 true 반환
	}
	static inline bool IsAllStop(CSphere r1, CSphere r2, CSphere w, CSphere y){
		return r1.isStop() && r2.isStop() && w.isStop() && y.isStop();
		//4개의 공이 모두 정지해 있을 경우 true 반환
	}
private:

	D3DXVECTOR3				velocity;
	float                   m_radius;

	LPD3DXEFFECT LoadShader(IDirect3DDevice9* pDevice, const char* fileName);
	LPDIRECT3DTEXTURE9 LoadTexture(IDirect3DDevice9* pDevice, const char* fileName);
	LPD3DXMESH createMesh(IDirect3DDevice9* pDevice, float rad, UINT slices, UINT stacks);

	void moveCenter(D3DXVECTOR3 velocity);
};
