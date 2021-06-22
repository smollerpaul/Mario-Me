#include "PlayScene.h"
#include "SolidBlock.h"
#include "GhostBlock.h"
#include "QuestionBlock.h"
#include "Goomba.h"
#include "RedGoomba.h"
#include "Koopas.h"
#include "WingedKoopas.h"
#include "Coin.h"


void CPlayScene::LoadMapObjects(string objectType, float x, float y, float width, float height)
{
	//mario spawn point
	if (objectType.compare("SpawnPoint") == 0) {
		player->SetPosition(x, y);
	}
	if (objectType.compare("SolidBlock") == 0) {
		SolidBlock* solidObject = new SolidBlock();
		solidObject->SetPosition(x, y);
		solidObject->SetSize(width, height);
		AddObject(solidObject);
	}
	if (objectType.compare("GhostBlock") == 0) {
		GhostBlock* ghostObject = new GhostBlock();
		ghostObject->SetPosition(x, y);
		ghostObject->SetSize(width, height);
		AddObject(ghostObject);
	}

	if (objectType.compare("Goomba") == 0) {
		CGoomba* goomba = new CGoomba();
		//goomba->SetPositionBottom(x, y);
		goomba->SetPosition(x, y);
		AddObject(goomba);
	}

	if (objectType.compare("QuestionBlock") == 0) {
		QuestionBlock* qb = new QuestionBlock();
		qb->SetPosition(x, y);
		AddObject(qb);
	}

	if (objectType.compare("RedGoomba") == 0) {
		RedGoomba* rg = new RedGoomba();
		rg->SetPosition(x, y);
		AddObject(rg);
	}
	if (objectType.compare("RedKoopas") == 0) {
		CKoopas* kp = new CKoopas(new NormalKoopas);
		kp->SetPosition(x, y);
		AddObject(kp);
		//DebugOut(L"i am koopas\n");
	}
	if (objectType.compare("KoopasJumping") == 0) {
		CKoopas* kp = new CKoopas();
		kp->SetPosition(x, y);
		AddObject(kp);
		//DebugOut(L"i am jumping koopas\n");
	}
	if (objectType.compare("Coin") == 0) {
		Coin* kp = new Coin();
		kp->SetPosition(x, y);
		AddObject(kp);
		//DebugOut(L"i am jumping koopas\n");
	}

	// void, pipe
}
