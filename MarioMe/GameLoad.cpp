#include <fstream>
#include "Game.h"
#include "Utils.h"
#include "Sprites.h"
#include "Animations.h"
#include "Textures.h"
#include "PlayScene.h"
#include "tinyxml/tinyxml.h"
#include "WorldScene.h"
#include "Font.h"

#define MAX_GAME_LINE 1024

void CGame::Load(string gameFile)
{
	TiXmlDocument doc(gameFile.c_str());

	if (doc.LoadFile()) {
		TiXmlElement* root = doc.RootElement();

		TiXmlElement* gameContent = root->FirstChildElement("GameContent");
		TiXmlElement* gameResources = root->FirstChildElement("Resources");

		LoadTextures(gameResources);
		DebugOut(L"\n");

		LoadSprites(gameResources);
		DebugOut(L"\n");

		LoadAnimations(gameResources);
		DebugOut(L"\n");

		LoadFontSet();
		DebugOut(L"\n");

		LoadScenes(gameContent);

		doc.Clear();
	}
}

void CGame::LoadTextures(TiXmlElement* gameResources) {
	TiXmlElement* textures = gameResources->FirstChildElement("Textures");

	for (TiXmlElement* node = textures->FirstChildElement("Texture"); node != nullptr; node = node->NextSiblingElement("Texture")) {
		CTextures::GetInstance()->Add(node->Attribute("id"), ToLPCWSTR(node->Attribute("path")), D3DCOLOR_ARGB(0, 0, 0, 0));
	}

}

void CGame::LoadSprites(TiXmlElement* gameResources) {
	TiXmlElement* sprites = gameResources->FirstChildElement("Sprites");

	for (TiXmlElement* node = sprites->FirstChildElement("Sprite"); node != nullptr; node = node->NextSiblingElement("Sprite")) {
		string id = node->Attribute("id");
		string path = node->Attribute("path");

		TiXmlDocument* spriteDoc = new TiXmlDocument(path.c_str());

		if (spriteDoc->LoadFile()) {

			DebugOut(L"[LOAD SPRITE PATH] %s\n", ToLPCWSTR(path));
			TiXmlElement* spriteRoot = spriteDoc->RootElement();

			for (TiXmlElement* sprites = spriteRoot->FirstChildElement("Textures"); sprites != nullptr; sprites = sprites->NextSiblingElement("Textures")) {
				string textId = sprites->Attribute("textureId");
				LPDIRECT3DTEXTURE9 texture = CTextures::GetInstance()->Get(textId);

				for (TiXmlElement* sprite = sprites->FirstChildElement("Sprite"); sprite != nullptr; sprite = sprite->NextSiblingElement("Sprite")) {
					string spriteId = sprite->Attribute("id");
					int left = 0, top = 0, width = 0, height = 0, xPivot = 0, yPivot = 0;

					sprite->QueryIntAttribute("left", &left);
					sprite->QueryIntAttribute("top", &top);
					sprite->QueryIntAttribute("width", &width);
					sprite->QueryIntAttribute("height", &height);

					if (sprite->Attribute("xPivot") != NULL) 
						sprite->QueryIntAttribute("xPivot", &xPivot);

					if (sprite->Attribute("yPivot") != NULL) 
						sprite->QueryIntAttribute("yPivot", &yPivot);

					CSprites::GetInstance()->Add(spriteId, left*3, top*3, width * 3, height * 3, xPivot * 3, yPivot * 3, texture);
				}
			}
		}
		else
		{
			DebugOut(L"Failed to load file \"%s\"\n", ToLPCWSTR(path));
		}
	}
}

void CGame::LoadAnimations(TiXmlElement* gameResources) {
	TiXmlElement* animations = gameResources->FirstChildElement("Animations");

	for (TiXmlElement* node = animations->FirstChildElement("Animation"); node != nullptr; node = node->NextSiblingElement("Animation")) {
		string id = node->Attribute("id");
		string path = node->Attribute("path");

		TiXmlDocument* aniDoc = new TiXmlDocument(path.c_str());

		if (aniDoc->LoadFile()) {

			DebugOut(L"[LOAD ANIMATION PATH] %s\n", ToLPCWSTR(path));
			TiXmlElement* aniRoot = aniDoc->RootElement();

			CAnimations* animations = CAnimations::GetInstance();

			for (TiXmlElement* aniTex = aniRoot->FirstChildElement("Textures"); aniTex != nullptr; aniTex = aniTex->NextSiblingElement("Textures")) {
				for (TiXmlElement* anim = aniTex->FirstChildElement("Animation"); anim != nullptr; anim = anim->NextSiblingElement("Animation")) {

					string aniId = anim->Attribute("aniId");
					int frameTime = 0;
					anim->QueryIntAttribute("frameTime", &frameTime);

					LPANIMATION ani = new CAnimation(frameTime);

					for (TiXmlElement* aniFrame = anim->FirstChildElement("Sprite"); aniFrame != nullptr; aniFrame = aniFrame->NextSiblingElement("Sprite")) {
						string frameId = aniFrame->Attribute("id");

						CSprites::GetInstance()->Get(frameId);
						ani->Add(frameId);
						DebugOut(L"   [FRAME] %s [INTO]  %s\n", ToLPCWSTR(frameId), ToLPCWSTR(aniId));
					}
					animations->Add(aniId, ani);
				}
			}
		}
		else
		{
			DebugOut(L"Failed to load file \"%s\"\n", ToLPCWSTR(path));
		}
	}
}

void CGame::LoadGameHud(TiXmlElement* gameContent)
{
	TiXmlElement* gameHud = gameContent->FirstChildElement("Hud");
	string path = gameHud->Attribute("path");

	int left=0, top=0;
	gameHud->QueryIntAttribute("left", &left);
	gameHud->QueryIntAttribute("top", &top);
}

void CGame::LoadScenes(TiXmlElement* gameContent)
{
	TiXmlElement* scenes = gameContent->FirstChildElement("Scenes");

	for (TiXmlElement* scene = scenes->FirstChildElement("Scene"); scene != nullptr; scene = scene->NextSiblingElement("Scene")) {
		string sceneId = scene->Attribute("id");
		string type = scene->Attribute("type");
		string sceneFilePath = scene->Attribute("path");

		if (type.compare("PlayScene") == 0) {
			CScene* scene = new CPlayScene(sceneId, sceneFilePath);
			this->scenes[sceneId]=scene;

			DebugOut(L"[PLAYSCENE ADDED]: %s \n", ToLPCWSTR(sceneId));
		}

		if (type.compare("WorldScene") == 0) {
			CScene* scene = new WorldScene(sceneId, sceneFilePath);
			this->scenes[sceneId] = scene;

			DebugOut(L"[WORLDSCENE ADDED]: %s \n", ToLPCWSTR(sceneId));
		}
	}
	SwitchScene(scenes->Attribute("start"));
}

void CGame::SwitchScene(string scene_id)
{
	DebugOut(L"[INFO] Switching to scene:  %s\n", ToLPCWSTR(scene_id));

	if (scenes[current_scene]) {
		scenes[current_scene]->Unload();
	}

	current_scene = scene_id;
	LPSCENE s = scenes[scene_id];
	s->Load();
}

void CGame::LoadFontSet() {

	this->fontSet = new FontSet();

	vector<FontSprite> charList;
	std::string prefix = "spr-font-";

	for (char c = '0'; c <= '9'; ++c)
		charList.push_back(FontSprite{ c, prefix + c });

	for (char c = 'A'; c <= 'Z'; ++c)
		charList.push_back(FontSprite{ c, prefix + c });

	charList.push_back(FontSprite{ '!', "spr-font-exclamation-point" });

	fontSet->InitFontSet(charList);

	DebugOut(L"[LOADED] Font Set loaded!\n");
}
