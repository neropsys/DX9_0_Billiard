////////////////////////////////////////////////////////////////////////////////
//
// File: virtualLego.cpp
//
// Original Author: 박창현 Chang-hyeon Park, 
// Modified by Bong-Soo Sohn and Dong-Jun Kim
// 
// Originally programmed for Virtual LEGO. 
// Modified later to program for Virtual Billiard.
//        
////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"
#include "CSphere.h"
#include "CWall.h"
#include "CLight.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>

IDirect3DDevice9* Device = NULL;

// window size
const int Width  = 1024;
const int Height = 768;

// There are four balls
// initialize the position (coordinate) of each ball (ball0 ~ ball3)
const float spherePos[4][2] = { {-2.7f,0} , {+2.4f,0} , {3.3f,0} , {-2.7f,-0.9f}}; 
// initialize the color of each ball (ball0 ~ ball3)
const D3DXCOLOR sphereColor[4] = {d3d::RED, d3d::RED, d3d::YELLOW, d3d::WHITE};

// -----------------------------------------------------------------------------
// Transform matrices
// -----------------------------------------------------------------------------
D3DXMATRIX g_mWorld;
D3DXMATRIX g_mView;
D3DXMATRIX g_mProj;

#define M_RADIUS 0.21   // ball radius
#define PI 3.14159265
#define M_HEIGHT 0.01
#define DECREASE_RATE 0.9982

// -----------------------------------------------------------------------------
// Global variables
// -----------------------------------------------------------------------------
CWall	g_legoPlane;
D3DXPLANE g_Plane(.0f, 1.f, 0.f, 0.f);
CWall	g_legowall[4];
CSphere	g_sphere[4];
/*CSphere g_redSphere[2];
CSphere g_whiteSphere;
CSphere g_yellowSphere;*/
CSphere	g_target_blueball;
CLight	g_light;

double g_camera_pos[3] = {0.0, 5.0, -8.0};

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------


void destroyAllLegoBlock(void)
{
}

// initialization
bool Setup()
{
	int i;
	
    D3DXMatrixIdentity(&g_mWorld);
    D3DXMatrixIdentity(&g_mView);
    D3DXMatrixIdentity(&g_mProj);
		
	// create plane and set the position
    if (false == g_legoPlane.create(Device, -1, -1, 9, 0.03f, 6, d3d::GREEN)) return false;
    //g_legoPlane.setPosition(0.0f, -0.0006f / 5, 0.0f);
	
	// create walls and set the position. note that there are four walls
	if (false == g_legowall[0].create(Device, -1, -1, 9, 0.3f, 0.12f, d3d::DARKRED)) return false;
	g_legowall[0].setPosition(0.0f, 0.12f, 3.06f);
	if (false == g_legowall[1].create(Device, -1, -1, 9, 0.3f, 0.12f, d3d::DARKRED)) return false;
	g_legowall[1].setPosition(0.0f, 0.12f, -3.06f);
	if (false == g_legowall[2].create(Device, -1, -1, 0.12f, 0.3f, 6.24f, d3d::DARKRED)) return false;
	g_legowall[2].setPosition(4.56f, 0.12f, 0.0f);
	if (false == g_legowall[3].create(Device, -1, -1, 0.12f, 0.3f, 6.24f, d3d::DARKRED)) return false;
	g_legowall[3].setPosition(-4.56f, 0.12f, 0.0f);

	// create four balls and set the position
	for (i=0;i<4;i++) {
		if (false == g_sphere[i].create(Device, sphereColor[i])) return false;
		g_sphere[i].setCenter(spherePos[i][0], (float)M_RADIUS , spherePos[i][1]);
		g_sphere[i].setPower(0,0);
	}
	
	// create blue ball for set direction
    if (false == g_target_blueball.create(Device, d3d::BLUE)) return false;
	g_target_blueball.setCenter(.0f, (float)M_RADIUS , .0f);
	
	// light setting 
    D3DLIGHT9 lit;
    ::ZeroMemory(&lit, sizeof(lit));
    lit.Type         = D3DLIGHT_POINT;
    lit.Diffuse      = d3d::WHITE; 
	lit.Specular	 = d3d::WHITE;
	lit.Ambient = d3d::WHITE;
    lit.Position     = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
    lit.Range        = 100.0f;
    lit.Attenuation0 = 0.0f;
    lit.Attenuation1 = 0.9f;
    lit.Attenuation2 = 0.0f;
    if (false == g_light.create(Device, lit))
        return false;
	
	// Position and aim the camera.
	D3DXVECTOR3 pos(0.0f, 5.0f, -8.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 2.0f, 0.0f);
	D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &g_mView);
	
	// Set the projection matrix.
	D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 4,
        (float)Width / (float)Height, 1.0f, 100.0f);
	Device->SetTransform(D3DTS_PROJECTION, &g_mProj);
	
    // Set render states.
    Device->SetRenderState(D3DRS_LIGHTING, TRUE);
    Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
    Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	

	g_light.setLight(Device, g_mWorld);
	return true;
}

void Cleanup(void)
{
    //g_legoPlane.destroy();
	for(int i = 0 ; i < 4; i++) {
		g_legowall[i].destroy();
	}
    destroyAllLegoBlock();
    g_light.destroy();
}


// timeDelta represents the time between the current image frame and the last image frame.
// the distance of moving balls should be "velocity * timeDelta"
bool Display(float timeDelta)
{
	int i=0;
	int j = 0;


	if( Device )
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0x00afafaf, 1.0f, 0);
		Device->BeginScene();

		// check whether any two balls hit together and update the direction of balls
		for(i = 0 ;i < 4; i++){
			for(j = 0 ; j < 4; j++) {
				if(i >= j) {continue;}
				g_sphere[i].hitBy(g_sphere[j]);
			}
		}
		// update the position of each ball. during update, check whether each ball hit by walls.
		for( i = 0; i < 4; i++) {
			for(j = 0; j < 4; j++){ g_legowall[i].hitBy(g_sphere[j]); }
			g_sphere[i].ballUpdate(timeDelta);
		}
		//TODO : shadow mapping
		/*
		Device->SetRenderState(D3DRS_STENCILENABLE, true);
		Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		Device->SetRenderState(D3DRS_STENCILREF, 0x0);
		Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
		Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
		Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

		D3DXVECTOR4 lightDir(
			g_light.getPosition().x,
			g_light.getPosition().y,
			g_light.getPosition().z,
			0.f);
		for (i = 0; i < 4; i++){
			D3DXMATRIX S, T;

			D3DXMatrixShadow(&S, &lightDir, &g_Plane);
			D3DXMatrixTranslation(
				&T,
				g_legowall[i].getPosition().x,
				g_legowall[i].getPosition().y,
				g_legowall[i].getPosition().z);
			Device->SetTransform(D3DTS_WORLD, &(T * S));
		}
		for (i = 0; i < 4; i++){
			D3DXMATRIX S, T;

			D3DXMatrixShadow(&S, &lightDir, &g_Plane);
			D3DXMatrixTranslation(
				&T,
				g_sphere[i].getCenter().x,
				g_sphere[i].getCenter().y,
				g_sphere[i].getCenter().z);
			Device->SetTransform(D3DTS_WORLD, &(T * S));
		}
		D3DXMATRIX S, T;

		D3DXMatrixShadow(&S, &lightDir, &g_Plane);
		D3DXMatrixTranslation(
			&T,
			g_legoPlane.getPosition().x,
			g_legoPlane.getPosition().y,
			g_legoPlane.getPosition().z);
		Device->SetTransform(D3DTS_WORLD, &(T * S));

		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		D3DMATERIAL9 mtrl = d3d::InitMtrl(d3d::BLACK, d3d::BLACK, d3d::BLACK, d3d::BLACK, 0.0f);
		mtrl.Diffuse.a = 0.5f;
		Device->SetRenderState(D3DRS_ZENABLE, false);
		Device->SetMaterial(&mtrl);
		Device->SetTexture(0, 0);
		*/

		// draw plane, walls, and spheres
		g_legoPlane.draw(Device, g_mWorld, g_mView);
		for (i=0;i<4;i++) 	{
			g_legowall[i].draw(Device, g_mWorld, g_mView);
			g_sphere[i].tempdraw(Device, g_mWorld, g_mView, g_light.getPosition4());//  draw(Device, g_mWorld, g_mView);
		}
		g_target_blueball.draw(Device, g_mWorld, g_mView);

        g_light.draw(Device);
		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		Device->SetTexture( 0, NULL );

		//Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		//Device->SetRenderState(D3DRS_STENCILENABLE, false);
	}
	return true;
}

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool wire = false;
	static bool isReset = true;
	static float zoomlvl = D3DX_PI / 4;
	static int old_x = 0;
	static float rot_x = 0;
	static float rot_y = 0;
	static int old_y = 0;
	static enum { WORLD_MOVE, LIGHT_MOVE, BLOCK_MOVE } move = WORLD_MOVE;

	switch (msg) {
	case WM_DESTROY:
	{
		::PostQuitMessage(0);
		break;
	}
	case WM_KEYDOWN:
	{
		switch (wParam) {
		case VK_F1:
			g_target_blueball.setCenter(.0f, (float)M_RADIUS, .0f);
			break;
		case VK_ESCAPE:
			::DestroyWindow(hwnd);
			break;
		case VK_RETURN:
			if (NULL != Device) {
				wire = !wire;
				Device->SetRenderState(D3DRS_FILLMODE,
					(wire ? D3DFILL_WIREFRAME : D3DFILL_SOLID));
			}
			break;
		case VK_SPACE:{
			D3DXVECTOR3 targetpos = g_target_blueball.getCenter();
			D3DXVECTOR3	whitepos = g_sphere[3].getCenter();
			double theta = acos(sqrt(pow(targetpos.x - whitepos.x, 2)) / sqrt(pow(targetpos.x - whitepos.x, 2) +
				pow(targetpos.z - whitepos.z, 2)));		// 기본 1 사분면
			if (targetpos.z - whitepos.z <= 0 && targetpos.x - whitepos.x >= 0) { theta = -theta; }	//4 사분면
			if (targetpos.z - whitepos.z >= 0 && targetpos.x - whitepos.x <= 0) { theta = PI - theta; } //2 사분면
			if (targetpos.z - whitepos.z <= 0 && targetpos.x - whitepos.x <= 0){ theta = PI + theta; } // 3 사분면
			double distance = sqrt(pow(targetpos.x - whitepos.x, 2) + pow(targetpos.z - whitepos.z, 2));
			g_sphere[3].setPower(distance * cos(theta), distance * sin(theta));
			break;
		}

			break;
		}
	}

	case WM_MOUSEMOVE:
	{
		int new_x = LOWORD(lParam);
		int new_y = HIWORD(lParam);
		float dx;
		float dy;

		if (LOWORD(wParam) & MK_LBUTTON) {

			if (isReset) {
				isReset = false;
			}
			else {
				D3DXVECTOR3 vDist;
				D3DXVECTOR3 vTrans;
				D3DXMATRIX mTrans;
				D3DXMATRIX mX;
				D3DXMATRIX mY;

				switch (move) {
				case WORLD_MOVE:
					dx = (old_x - new_x) * 0.01f;
					dy = (old_y - new_y) * 0.01f;
					D3DXMatrixRotationY(&mX, dx);
					D3DXMatrixRotationX(&mY, dy);
					g_mWorld = g_mWorld * mX * mY;

					break;
				}
			}

			old_x = new_x;
			old_y = new_y;

		}
		else {
			isReset = true;

			if (LOWORD(wParam) & MK_RBUTTON) {
				if (LOWORD(wParam) <= 0)
					new_x = 0;
				if (new_y <= 0)
					new_y = 0;
				dx = (old_x - new_x);// * 0.01f;
				dy = (old_y - new_y);// * 0.01f;

				D3DXVECTOR3 coord3d = g_target_blueball.getCenter();
				g_target_blueball.setCenter(coord3d.x + dx*(-0.007f), coord3d.y, coord3d.z + dy*0.007f);
			}
			int new_x = LOWORD(lParam);
			int new_y = HIWORD(lParam);
			float dx;
			float dy;

			if (LOWORD(wParam) & MK_LBUTTON) {

				if (isReset) {
					isReset = false;
				}
				else {
					D3DXVECTOR3 vDist;
					D3DXVECTOR3 vTrans;
					D3DXMATRIX mTrans;
					D3DXMATRIX mX;
					D3DXMATRIX mY;

					switch (move) {
					case WORLD_MOVE:
						dx = (old_x - new_x) * 0.01f;
						dy = (old_y - new_y) * 0.01f;

						//rotation restriction
						/*
						rot_x += dx;
						rot_y += dy;
						std::string debug = "X :" + std::to_string(rot_x) + " Y: " + std::to_string(rot_y) + "\n";
						OutputDebugString(debug.c_str());
						if (abs(rot_y) > 1.0 || abs(rot_x) > 1.7f)
						break;
						*/
						D3DXMatrixRotationY(&mX, dx);
						D3DXMatrixRotationX(&mY, dy);
						g_mWorld = g_mWorld * mX * mY;

						break;
					}
				}

				old_x = new_x;
				old_y = new_y;

			}
			else {
				isReset = true;

				if (LOWORD(wParam) & MK_RBUTTON) {
					dx = (old_x - new_x);// * 0.01f;
					dy = (old_y - new_y);// * 0.01f;

					D3DXVECTOR3 coord3d = g_target_blueball.getCenter();
					g_target_blueball.setCenter(coord3d.x + dx*(-0.007f), coord3d.y, coord3d.z + dy*0.007f);
				}
				old_x = new_x;
				old_y = new_y;

				move = WORLD_MOVE;
			}
			break;
		}
	}
	case WM_MOUSEWHEEL:
	{
		short wheelAmout = GET_WHEEL_DELTA_WPARAM(wParam);
		if (wheelAmout != 0){
			D3DXMATRIX mProj;
			if (wheelAmout > 0){
				zoomlvl += 0.1f;
				if (zoomlvl >= 1.f)
					zoomlvl = 1.f;
			}
			else{
				zoomlvl -= 0.1f;
				if (zoomlvl <= 0.2f)
					zoomlvl = 0.2f;
			}

			D3DXMatrixPerspectiveFovLH(&mProj, zoomlvl, (float)Width / (float)Height, 1.f, 100.0f);
			Device->SetTransform(D3DTS_PROJECTION, &mProj);
		}
		break;
	}
	}

	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
    srand(static_cast<unsigned int>(time(NULL)));
	
	if(!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}
	
	if(!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}
	
	d3d::EnterMsgLoop( Display );
	
	Cleanup();
	
	Device->Release();
	
	return 0;
}