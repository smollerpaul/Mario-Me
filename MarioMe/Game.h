#pragma once

#include <unordered_map>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>


#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Scene.h"
#include "tinyXML/tinyxml.h"

using namespace std;

#define KEYBOARD_BUFFER_SIZE	1024

class Keyboard;
class CGame
{
	static CGame * __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;		
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	Keyboard* keyboard;
	int screen_width;
	int screen_height; 

	unordered_map<string, LPSCENE> scenes;
	string current_scene; 

	/*void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);*/

public:

	Keyboard* GetKeyboard();

	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, 
		int left, int top, int right, int bottom, int alpha = 255, int flip = 1);

	void Load(string gameFile);
	void LoadTextures(TiXmlElement* gameResources);
	void LoadSprites(TiXmlElement* gameResources);
	void LoadAnimations(TiXmlElement* gameResources);

	void LoadGameHud(TiXmlElement* gameContent);
	void LoadScenes(TiXmlElement* gameContent);

	
	LPSCENE GetCurrentScene();
	void SwitchScene(string scene_id);

	int GetScreenWidth();
	int GetScreenHeight();

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			
		float dy,			
		float sl,			// static left
		float st, 
		float sr, 
		float sb,
		float &t, 
		float &nx, 
		float &ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice();
	LPDIRECT3DSURFACE9 GetBackBuffer();
	LPD3DXSPRITE GetSpriteHandler();

	static CGame * GetInstance();

	~CGame();
};


