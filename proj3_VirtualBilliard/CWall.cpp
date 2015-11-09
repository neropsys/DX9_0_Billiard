#include "CWall.h"
#include "CSphere.h"
#include <d3dx9math.h>

CWall::CWall(){
	D3DXMatrixIdentity(&m_mLocal);
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
	m_width = 0;
	m_depth = 0;
	m_pMesh = NULL;
}
CWall::~CWall(){}

bool CWall::create(IDirect3DDevice9* pDevice,
	float ix,
	float iz,
	float iwidth,
	float iheight,
	float idepth,
	D3DXCOLOR color){
	if (!CObject::create(pDevice, color))
		return false;

	m_width = iwidth;
	m_depth = idepth;
	
	if (FAILED(D3DXCreateBox(pDevice, iwidth, iheight, idepth, &m_pMesh, NULL)))
		return false;
	return true;
}

void CWall::destroy(){
	if (m_pMesh != NULL){
		m_pMesh->Release();
		m_pMesh = NULL;
	}
		
}
void CWall::draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld,
	const D3DXMATRIX& mView){
	if (NULL == pDevice)
		return;
	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
	pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
	pDevice->SetMaterial(&m_mtrl);
	m_pMesh->DrawSubset(0);
}
//Determines whether the ball has intersected the wall
bool CWall::hasIntersected(CSphere& ball){
	// There are two ways to determine intersection
	// |z - z'| <= r		|		|x - x'| <= r
	// |z + vzt - z'| <= r	|		|x + vxt - x'| <= r
	// After Collision
	// z = z' - r | x = x' - r
	
	D3DXVECTOR3 cord = ball.getCenter();
	
	if (cord.x >= (4.5 - M_RADIUS))  return true;
		//cord.x = 4.5 - M_RADIUS;
	if (cord.x <= (-4.5 + M_RADIUS)) return true;
		//cord.x = -4.5 + M_RADIUS;
	if (cord.z <= (-3 + M_RADIUS))	 return true;
		//cord.z = -3 + M_RADIUS;
	if (cord.z >= (3 - M_RADIUS))	 return true;
		//cord.z = 3 - M_RADIUS;
	
	return false;
}
//changes x-velocity and z-velocity of the ball after collision
void CWall::hitBy(CSphere& ball){
	
	if (!this->hasIntersected(ball)) return;

	D3DXVECTOR3 cord = ball.getCenter();

	if (cord.x >= (4.5 - M_RADIUS)) // 공의 X값이 우측으로 치우친 경우 ( 우측 벽에 부딪힌 경우 )
	{
		ball.setCenter(4.5 - M_RADIUS, cord.y, cord.z);
		ball.setPower(ball.getVelocity().x*(-0.5), ball.getVelocity().z); // 방향 전환
	}
	if (cord.x <= (-4.5 + M_RADIUS)) // 공의 X값이 좌측으로 치우친 경우 ( 좌측 벽에 부딪힌 경우 )
	{
		ball.setCenter(-4.5 + M_RADIUS, cord.y, cord.z);
		ball.setPower(ball.getVelocity().x*(-0.5), ball.getVelocity().z); // 방향 전환
	}
	if (cord.z <= (-3 + M_RADIUS)) // 공의 Z값이 아래로 치우친 경우 ( 하측 벽에 부딪힌 경우 )
	{
		ball.setCenter(cord.x, cord.y, -3 + M_RADIUS);
		ball.setPower(ball.getVelocity().x, ball.getVelocity().z*(-0.5)); // 방향 전환
	}
	if (cord.z >= (3 - M_RADIUS))  // 공의 Z값이 위로 치우친 경우 ( 상측 벽에 부딪힌 경우 )
	{
		ball.setCenter(cord.x, cord.y, 3 - M_RADIUS);
		ball.setPower(ball.getVelocity().x, ball.getVelocity().z*(-0.5)); // 방향 전환
	}
}

void CWall::setPosition(float x, float y, float z)
{
	D3DXMATRIX m;
	this->m_x = x;
	this->m_z = z;

	D3DXMatrixTranslation(&m, x, y, z);
	setLocalTransform(m);
}
