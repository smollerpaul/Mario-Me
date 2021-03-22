#include <iostream>
#include <fstream>

#include "Game.h"
#include "Utils.h"

#include "PlayScene.h"
#include "Keyboard.h"
#include "Camera.h"

CGame * CGame::__instance = NULL;

/*
	Initialize DirectX, create a Direct3D device for rendering within the window, initial Sprite library for 
	rendering 2D images
	- hInst: Application instance handle
	- hWnd: Application window handle
*/
Keyboard* CGame::GetKeyboard()
{
	if (keyboard == nullptr) {
		keyboard = new Keyboard();
		keyboard->Init(hWnd);
	}
		
	return keyboard;
}

void CGame::Init(HWND hWnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWnd = hWnd;									

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;

	RECT r;
	GetClientRect(hWnd, &r);	// retrieve Window width & height 

	d3dpp.BackBufferHeight = r.bottom + 1;
	d3dpp.BackBufferWidth = r.right + 1;

	screen_height = r.bottom + 1;
	screen_width = r.right + 1;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (d3ddv == NULL)
	{
		OutputDebugString(L"[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite helper from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);

	OutputDebugString(L"[INFO] InitGame done;\n");
}

void CGame::Draw(float x, float y,int flip, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha)
{
	RECT r;
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;

	D3DXVECTOR3 p(x, y, 0);

	//D3DXVECTOR3 center(x, y, 0);

	D3DXMATRIX oldMatrix, scale;

	D3DXVECTOR2 transformCenter = D3DXVECTOR2(x + (r.right - r.left) / 2, y + (r.bottom - r.top) / 2);
	D3DXMatrixTransformation2D(&scale, 
		&transformCenter, 0, &D3DXVECTOR2(flip* 1.0f, 1.0f),
		&transformCenter, 0.0f, 
		&D3DXVECTOR2(0,0));

	spriteHandler->GetTransform(&oldMatrix);
	spriteHandler->SetTransform(&scale);
	spriteHandler->Draw(texture, &r, NULL, &p, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	spriteHandler->SetTransform(&oldMatrix);
}


CGame::~CGame()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
}

CGame *CGame::GetInstance()
{
	if (__instance == NULL) __instance = new CGame();
	return __instance;
}

int CGame:: GetScreenWidth() { return screen_width; }
int CGame:: GetScreenHeight() { return screen_height; }

LPSCENE CGame:: GetCurrentScene() { return scenes[current_scene]; }

LPDIRECT3DDEVICE9 CGame::GetDirect3DDevice() { return this->d3ddv; }
LPDIRECT3DSURFACE9 CGame::GetBackBuffer() { return backBuffer; }
LPD3DXSPRITE CGame:: GetSpriteHandler() { return this->spriteHandler; }
