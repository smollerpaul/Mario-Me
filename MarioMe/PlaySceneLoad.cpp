#include "PlayScene.h"
#include "SolidBlock.h"
#include "GhostBlock.h"
#include "QuestionBlock.h"
#include "Goomba.h"
#include "RedGoomba.h"
#include "Koopas.h"
#include "WingedKoopas.h"
#include "Coin.h"

//chinh qblock

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
		goomba->SetPosition(x, y);
		AddObject(goomba);
	}

	
	if (objectType.compare("RaccoonLeaf") == 0) {
		QuestionBlock* qb = new QuestionBlock(LEAF_PRIZE);
		qb->SetPosition(x, y);
		AddObject(qb);
		DebugOut(L"RacoonLeaf created!\n");
	}
	if (objectType.compare("GreenMushroom") == 0) {
		QuestionBlock* qb = new QuestionBlock(GMUSH_PRIZE);
		qb->SetPosition(x, y);
		AddObject(qb);
		DebugOut(L"GreenMushroom created!\n");
	}

	if (objectType.compare("QuestionBlock") == 0) {
		QuestionBlock* qb = new QuestionBlock();
		qb->SetPosition(x, y);
		AddObject(qb);
		DebugOut(L"QuestionBlock created!\n");
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
	}
	if (objectType.compare("KoopasJumping") == 0) {
		CKoopas* kp = new CKoopas();
		kp->SetPosition(x, y);
		AddObject(kp);
	}
	if (objectType.compare("Coin") == 0) {
		Coin* kp = new Coin();
		kp->SetPosition(x, y);
		AddObject(kp);
	}

	// void, pipe
}
