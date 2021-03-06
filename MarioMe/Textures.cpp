#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "Utils.h"
#include "Game.h"
#include "textures.h"

CTextures * CTextures::__instance = NULL;

CTextures::CTextures()
{

}

CTextures *CTextures::GetInstance()
{
	if (__instance == NULL) __instance = new CTextures();
	return __instance;
}

void CTextures::Add(string id, LPCWSTR filePath, D3DCOLOR transparentColor)
{
	textures[id] = Load(filePath, transparentColor);

	DebugOut(L"[TEXTURE] Texture added: id=%s, %s\n", ToLPCWSTR(id), filePath); // %d la decimal, %s la string
}

LPDIRECT3DTEXTURE9 CTextures::Get(string id) 
{
	return textures[id];
}

LPDIRECT3DTEXTURE9 CTextures::Load(LPCWSTR path, D3DCOLOR transparentColor)
{
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(path, &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", path);
		return nullptr;
	}

	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();
	LPDIRECT3DTEXTURE9 texture;

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		path,							// Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transparentColor,
		&info,
		NULL,
		&texture);								// Created texture pointer

	if (result != D3D_OK)
	{
		OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
		return nullptr;
	}
	return texture;
}


void CTextures::Clear()
{
	for (auto x : textures)
	{
		LPDIRECT3DTEXTURE9 tex = x.second; 
		if (tex!=NULL) tex->Release();
	}
	
	textures.clear();
}



