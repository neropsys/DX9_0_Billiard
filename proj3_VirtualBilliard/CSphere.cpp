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

//need to use const &
float dotproduct(CSphere a, CSphere b)
{
	return a.getVelocity_X() * b.getVelocity_X() + a.getVelocity_Z()*b.getVelocity_Z();
}

float dotproduct(float ax, float az, float bx, float bz)
{
	return ax* bx + az*bz;
}

//calculate absolute value of given vector( = |u| )
float abs_vector(float x, float y){
	return sqrtf(pow(x, 2) + pow(y, 2));
}

//seperating balls after collision
void separate(CSphere& ball_a, CSphere& ball_b)
{
	//get coordinates for both balls
	D3DXVECTOR3 cord_a = ball_a.getCenter();
	D3DXVECTOR3 cord_b = ball_b.getCenter();
	// 1. define vector v and v_a, v_b( ball_b의 중심좌표 - ball_a의 중심좌표로 v를 계산하고 공 a와 b의 벡터를 정의 )
	float v_x = cord_b.x - cord_a.x;
	float v_z = cord_b.z - cord_a.z;
	// 2. find direction of vector( 벡터v의 위치벡터가 1, 2, 3, 4사분면 중에서 어느 사분면에 속하는지 판단 )
	float direction_x, direction_z;
	v_x > 0 ? direction_x = 1 : direction_x = -1;
	v_z > 0 ? direction_z = 1 : direction_z = -1;
	// 3. calculate theta with vector v( 벡터 v가 x축과 이루는 각도 세타theta 계산 )
	float theta = atan(v_z/v_x);	
	// 4. find a distance a ball has to move( 공이 움직여야 하는 거리 계산 )
	float length = (sqrtf(pow(v_x, 2) + pow(v_z, 2))); // 중심거리
	float distance = M_RADIUS - (length / 2);
	// 5. find △x, △z using theta( 세타를 이용해서 델타 x와 델타 z를 구한다 )
	float delta_x = abs(distance*cos(theta));
	float delta_z = abs(distance*sin(theta));
	// 6. get new position of the ball using △x, △z and vector u( 델타 x, z와 단위 벡터 u를 이용해 공의 나중 좌표를 계산한다 )
	ball_a.setCenter(cord_a.x + delta_x*(-1)*direction_x, cord_a.y, cord_a.z + delta_z*(-1)*direction_z);
	ball_b.setCenter(cord_b.x + delta_x*direction_x, cord_b.y, cord_b.z + delta_z*direction_z);
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

		separate(*this, ball);
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
