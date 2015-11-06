//#include "CSphere.h"
//#include <d3d9.h>
//CSphere::CSphere(){
//	D3DXMatrixIdentity(&m_mLocal);
//	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
//	m_radius = 0;
//	m_velocity_x = 0;
//	m_velocity_z = 0;
//	m_pMesh = NULL;
//}
//CSphere::~CSphere(){}
//bool CSphere::create(IDirect3DDevice9* pDevice, D3DXCOLOR color){
//	if (!CObject::create(pDevice, color))
//		return false;
//	/*
//	m_effect = LoadShader(pDevice, SPHERE_VS_NAME);
//	if (!m_effect)
//		return false;
//
//	m_texture = LoadTexture(pDevice, SPHERE_TEXTURE);
//
//	if (!m_texture)
//		return false;
//		*/
//	if (FAILED(D3DXCreateSphere(pDevice, getRadius(), 50, 50, &m_pMesh, NULL)))
//		return false;
//	return true;
//}
//void CSphere::destroy(){
//
//	if (m_pMesh != NULL) {
//		m_pMesh->Release();
//		m_pMesh = NULL;
//	}
//	if (m_effect != NULL){
//		m_effect->Release();
//		m_effect = NULL;
//	}
//	if (m_texture != NULL){
//		m_texture->Release();
//		m_texture = NULL;
//	}
//}
//void CSphere::draw(IDirect3DDevice9* pDevice,
//	const D3DXMATRIX& mWorld,
//	const D3DXMATRIX& mView,
//	const D3DXMATRIX& mProj)
//{
//	if (NULL == pDevice)
//		return;
//	
//	pDevice->SetTransform(D3DTS_WORLD, &mWorld);
//	pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
//	pDevice->SetMaterial(&m_mtrl);
//	/*
//	m_effect->SetMatrix("gWorld", &mWorld);
//	m_effect->SetMatrix("gView", &mView);
//	m_effect->SetMatrix("gProj", &mProj);
//	m_effect->SetTexture("DiffuseSampler", m_texture);
//
//	UINT numPass = 0;
//	m_effect->Begin(&numPass, NULL);
//	{
//		for (UINT i = 0; i < numPass; ++i){
//			//m_effect->BeginPass(i)
//			{
//				m_pMesh->DrawSubset(0);
//			}
//			//m_effect->
//		}
//	}
//	m_effect->End();
//	*/
//	m_pMesh->DrawSubset(0);
//
//}
//bool CSphere::hasIntersected(CSphere& ball)
//{
//	// Insert your code here.
//	return false;
//}
//void CSphere::hitBy(CSphere& ball)
//{
//	// Insert your code here.
//}
//void CSphere::ballUpdate(float timeDiff)
//{
//	const float TIME_SCALE = 3.3;
//	D3DXVECTOR3 cord = this->getCenter();
//	double vx = abs(this->getVelocity_X());
//	double vz = abs(this->getVelocity_Z());
//
//	if (vx > 0.01 || vz > 0.01)
//	{
//		float tX = cord.x + TIME_SCALE*timeDiff*m_velocity_x;
//		float tZ = cord.z + TIME_SCALE*timeDiff*m_velocity_z;
//
//		//correction of position of ball
//		/* Please uncomment this part because this correction of ball position is necessary when a ball collides with a wall
//		if (tX >= (4.5 - M_RADIUS))
//			tX = 4.5 - M_RADIUS;
//		if (tX <= (-4.5 + M_RADIUS))
//			tX = -4.5 + M_RADIUS;
//		if (tZ <= (-3 + M_RADIUS))
//			tZ = -3 + M_RADIUS;
//		if (tZ >= (3 - M_RADIUS))
//			tZ = 3 - M_RADIUS;
//		*/
//		this->setCenter(tX, cord.y, tZ);
//
//	}
//	else { this->setPower(0, 0); }
//	//this->setPower(this->getVelocity_X() * DECREASE_RATE, this->getVelocity_Z() * DECREASE_RATE);
//	double rate = 1 - (1 - DECREASE_RATE)*timeDiff * 400;
//	if (rate < 0)
//		rate = 0;
//	this->setPower(getVelocity_X() * rate, getVelocity_Z() * rate);
//}
//void CSphere::setCenter(float x, float y, float z){
//	setPosition(x, y, z);
//}
//void CSphere::setPosition(float x, float y, float z){
//	D3DXMATRIX m;
//	m_x = x;	m_y = y;	m_z = z;
//	D3DXMatrixTranslation(&m, x, y, z);
//	setLocalTransform(m);
//}
//LPD3DXEFFECT CSphere::LoadShader(IDirect3DDevice9* pDevice, const char* fileName){
//	LPD3DXEFFECT ret = NULL;
//	LPD3DXBUFFER pError = NULL;
//	DWORD dwShaderFlags = 0;
//#if _DEBUG
//	dwShaderFlags |= D3DXSHADER_DEBUG;
//#endif
//	D3DXCreateEffectFromFile(pDevice, fileName, NULL, NULL, dwShaderFlags, NULL, &ret, &pError);
//	if (!ret && pError){
//		int size = pError->GetBufferSize();
//		void* ack = pError->GetBufferPointer();
//		if (ack){
//			char* str = new char[size];
//			sprintf(str, (const char*)ack, size);
//			OutputDebugString(str);
//			delete[] str;
//		}
//	}
//	return ret;
//
//}
//LPDIRECT3DTEXTURE9 CSphere::LoadTexture(IDirect3DDevice9* pDevice, const char* fileName){
//	LPDIRECT3DTEXTURE9 ret = NULL;
//	if (FAILED(D3DXCreateTextureFromFile(pDevice, fileName, &ret))){
//		OutputDebugString("Failed to load texture: ");
//		OutputDebugString(fileName);
//		OutputDebugString("\n");
//	}
//	return ret;
//}

#include "CSphere.h"

#include <d3d9.h>



CSphere::CSphere() {

	D3DXMatrixIdentity(&m_mLocal);
	ZeroMemory(&m_mtrl, sizeof(m_mtrl));
	m_radius = 0;
	m_velocity_x = 0;
	m_velocity_z = 0;
	m_pMesh = NULL;

}

CSphere::~CSphere() {}

bool CSphere::create(IDirect3DDevice9* pDevice, D3DXCOLOR color) {

	if (!CObject::create(pDevice, color))
		return false;
	/*
	m_effect = LoadShader(pDevice, SPHERE_VS_NAME);
	if (!m_effect)
	return false;
	m_texture = LoadTexture(pDevice, SPHERE_TEXTURE);
	if (!m_texture)
	return false;
	*/
	if (FAILED(D3DXCreateSphere(pDevice, getRadius(), 50, 50, &m_pMesh, NULL)))
		return false;
	return true;
}
void CSphere::destroy() {

	if (m_pMesh != NULL) {
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	if (m_effect != NULL) {
		m_effect->Release();
		m_effect = NULL;
	}
	if (m_texture != NULL) {
		m_texture->Release();
		m_texture = NULL;
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
	pDevice->SetMaterial(&m_mtrl);
	/*
	m_effect->SetMatrix("gWorld", &mWorld);
	m_effect->SetMatrix("gView", &mView);
	m_effect->SetMatrix("gProj", &mProj);
	m_effect->SetTexture("DiffuseSampler", m_texture);

	UINT numPass = 0;
	m_effect->Begin(&numPass, NULL);
	{
	for (UINT i = 0; i < numPass; ++i){
	//m_effect->BeginPass(i)
	{
	m_pMesh->DrawSubset(0);
	}
	//m_effect->
	}
	}
	m_effect->End();
	*/
	m_pMesh->DrawSubset(0);

}



bool CSphere::hasIntersected(CSphere& ball)
{
	float distance = 0;
	distance = sqrtf((pow(this->getCenter().x - ball.getCenter().x, 2)) + (pow((this->getCenter().z - ball.getCenter().z), 2)));
	//프레임 위치 이용한 계싼 
	if (distance < 2 * (this->getRadius())) return true; //겹친거니까 
	else									return false;
}

/*
범위를 절대값
우준혁이 hitby코드를 하면
그것을 바로 intersected 를 부르는데 이게 true면 if 문 안에서 좌표변경이고 아니면
그냥 continue !!
충돌후 나온 결과 4가지가 있느데
v1(v1'x , v1'y)
v2 ( v2'x, v2'y)
o1(x1,y1) //
o2(x2,y2)

o' o2 다음 프레임 위치는 알겟지 수식잇어 잘 봐...
거리 계산해서 거리 <= 나 < 는 상관없음ㅎㅎ
*/


float dotproduct(CSphere a, CSphere b)
{
	return a.getVelocity_X() * b.getVelocity_X() + a.getVelocity_Z()*b.getVelocity_Z();
}

float dotproduct(float ax, float az, float bx, float bz)
{
	return ax* bx + az*bz;
}

void CSphere::hitBy(CSphere& ball) {
	if (hasIntersected(ball))
	{

		CSphere a, b;
		float normVx, normVz, tmp1, tmp2;
		tmp1 = this->getVelocity_X(); tmp2 = this->getVelocity_Z();
		normVx = this->getVelocity_X() - ball.getVelocity_X();
		normVz = this->getVelocity_Z() - ball.getVelocity_Z();
		normVx /= sqrtf(normVx * normVx + normVz*normVz);
		normVz /= sqrtf(normVx * normVx + normVz*normVz);

		a.setPower(normVx, normVz); b.setPower(normVz, -normVx);

		this->setPower(dotproduct(ball, a) * normVx + dotproduct(*this, b) * normVz,
			dotproduct(ball, a) * normVz - dotproduct(*this, b) * normVx);

		ball.setPower(dotproduct(*this, a) * normVx + dotproduct(ball, b) * normVz,
			dotproduct(tmp1, tmp2, normVx, normVz) * normVz - dotproduct(ball.getVelocity_X(), ball.getVelocity_Z(), normVz, -normVx) * normVx);

		/*
		D3DXVECTOR3 cord1 = this->getCenter();
		D3DXVECTOR3 cord2 = ball.getCenter();
		
		float distance = sqrtf((pow(cord1.x - cord2.x, 2)) + (pow((cord1.z - cord2.z), 2)));
		float shift = M_RADIUS - distance / 2; 
		this->setCenter(cord1.x + this->getVelocit_X()*,
			cord1.y, cord1.z + this->getVelocity_Z());
		ball.setCenter(cord2.x + ball.getVelocity_X(),
			cord2.y, cord2.z + ball.getVelocity_Z());
		if (b1.hasIntersected(b2)) {
			b1.setCenter(b1.getCenter().x, b1.getCenter().y, b1.getCenter().z);
			//hk dnlcl dhfarudigka 
			b2.setCenter(b2.getCenter().x + 1, b2.getCenter().y + 1, b2.getCenter().z + 3);

		}
		*/
	}
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
		/* Please uncomment this part because this correction of ball position is necessary when a ball collides with a wall
		if (tX >= (4.5 - M_RADIUS))
			tX = 4.5 - M_RADIUS;
		else if (tX <= (-4.5 + M_RADIUS))
			tX = -4.5 + M_RADIUS;
		else if (tZ <= (-3 + M_RADIUS))
			tZ = -3 + M_RADIUS;
		else if (tZ >= (3 - M_RADIUS))
			tZ = 3 - M_RADIUS;
		*/
		this->setCenter(tX, cord.y, tZ);
	}
	else { this->setPower(0, 0); }
	//this->setPower(this->getVelocity_X() * DECREASE_RATE, this->getVelocity_Z() * DECREASE_RATE);
	double rate = 1 - (1 - DECREASE_RATE)*timeDiff * 400;
	if (rate < 0)
		rate = 0;
	this->setPower(getVelocity_X() * rate, getVelocity_Z() * rate);
}

void CSphere::setCenter(float x, float y, float z) {

	setPosition(x, y, z);

}

void CSphere::setPosition(float x, float y, float z) {
	D3DXMATRIX m;
	m_x = x;	m_y = y;	m_z = z;
	D3DXMatrixTranslation(&m, x, y, z);
	setLocalTransform(m);
}

LPD3DXEFFECT CSphere::LoadShader(IDirect3DDevice9* pDevice, const char* fileName) {
	LPD3DXEFFECT ret = NULL;
	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;

#endif
	D3DXCreateEffectFromFile(pDevice, fileName, NULL, NULL, dwShaderFlags, NULL, &ret, &pError);
	if (!ret && pError) {
		int size = pError->GetBufferSize();
		void* ack = pError->GetBufferPointer();
		if (ack) {
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
		}
	}
	return ret;

}

LPDIRECT3DTEXTURE9 CSphere::LoadTexture(IDirect3DDevice9* pDevice, const char* fileName) {
	LPDIRECT3DTEXTURE9 ret = NULL;
	if (FAILED(D3DXCreateTextureFromFile(pDevice, fileName, &ret))) {
		OutputDebugString("Failed to load texture: ");
		OutputDebugString(fileName);
		OutputDebugString("\n");
	}
	return ret;
}
