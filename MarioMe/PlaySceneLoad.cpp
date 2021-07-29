#include "PlayScene.h"
#include "SolidBlock.h"
#include "GhostBlock.h"
#include "QuestionBlock.h"
#include "Goomba.h"
#include "RedGoomba.h"
#include "Koopas.h"
#include "WingedKoopas.h"
#include "Coin.h"
#include "EndCard.h"
#include "Void.h"
#include "Pipe.h"
#include "Venus.h"
#include "Brick.h"
#include "PSwitch.h"
#include "Piranha.h"
#include "KoopasBound.h"
#include "BoomBound.h"
#include "RedNormalKoopas.h"
#include "StrawCube.h"
#include "MarioConstants.h"
#include "MusicNote.h"
#include "CoinBrick.h"
#include "BoomBro.h"
#include "BeginPortal.h"
#include "EndPortal.h"
#include "Grid.h"
#include "WingedGoomba.h"

void CPlayScene::LoadMapObjects(string objectType, vector<D3DXVECTOR2> cells, float x, float y, float width, float height)
{
	if (objectType.compare("SpawnPoint") == 0) {
		player->SetPosition(x, y);
	}

	if (objectType.compare("SecretMusicNote") == 0) {
		QuestionBlock* qb = new QuestionBlock(this);
		qb->SetReward(SECRET_MUSIC_PRIZE);
		qb->SetPosition(x, y);
		//AddObject(qb);
		this->grid->Add(qb, cells);
		DebugOut(L"added secret mnote!\n");
	}

	if (objectType.compare("Venus") == 0) {
		Venus* kp = new Venus(this);
		kp->SetPosition(x, y);
		//AddObject(kp);
		this->grid->Add(kp, cells);
	}

	if (objectType.compare("RedVenus") == 0) {
		Venus* kp = new Venus(this);
		kp->SetPosition(x, y);
		kp->isRedVenus = 1;
		//AddObject(kp);
		this->grid->Add(kp, cells);
	}

	if (objectType.compare("Piranha") == 0) {
		Piranha* kp = new Piranha();
		kp->SetPosition(x, y);
		//AddObject(kp);
		this->grid->Add(kp, cells);
	}

	if (objectType.compare("SolidBlock") == 0) {
		SolidBlock* solidObject = new SolidBlock();
		solidObject->SetPosition(x, y);
		solidObject->SetSize(width, height);
		//AddObject(solidObject);
		this->grid->Add(solidObject, cells);
	}

	if (objectType.compare("GhostBlock") == 0) {
		GhostBlock* ghostObject = new GhostBlock();
		ghostObject->SetPosition(x, y);
		ghostObject->SetSize(width, height);
		//AddObject(ghostObject);
		this->grid->Add(ghostObject, cells);
	}

	if (objectType.compare("KoopasBound") == 0) {
		KoopasBound* kpB = new KoopasBound();
		kpB->SetPosition(x, y);
		kpB->SetSize(width, height);
		//AddObject(kpB);
		this->grid->Add(kpB, cells);
	}

	if (objectType.compare("BoomBound") == 0) {
		BoomBound* bbb = new BoomBound();
		bbb->SetPosition(x, y);
		bbb->SetSize(width, height);
		//AddObject(bbb);
		this->grid->Add(bbb, cells);
	}

	if (objectType.compare("BoomBro") == 0) {
		BoomBro* bbb = new BoomBro(this);
		bbb->SetPosition(x, y);
		//AddObject(bbb);
		this->grid->Add(bbb, cells);
	}

	if (objectType.compare("Brick") == 0) {
		CBrick* bb = new CBrick();
		bb->SetPosition(x, y);
		//AddObject(bb);
		this->grid->Add(bb, cells);
	}

	if (objectType.compare("Void") == 0) {
		Void* v = new Void();
		v->SetPosition(x, y);
		v->SetSize(width, height);
		//AddObject(v);
		this->grid->Add(v, cells);
	}

	if (objectType.compare("Goomba") == 0) {
		CGoomba* goomba = new CGoomba();
		goomba->SetPosition(x, y);
		//AddObject(goomba);
		this->grid->Add(goomba, cells);
	}

	if (objectType.compare("FlyingGoomba") == 0) {
		CGoomba* goomba = new CGoomba();
		goomba->SetObjectState(new WingedGoomba(goomba));
		goomba->SetPosition(x, y);
		//AddObject(goomba);
		this->grid->Add(goomba, cells);
	}

	if (objectType.compare("PSwitch") == 0) {
		QuestionBlock* qb = new QuestionBlock(this);
		qb->SetReward(PSWITCH_PRIZE);
		qb->SetAsBrick(1);
		qb->SetPosition(x, y);
		//AddObject(qb);
		this->grid->Add(qb, cells);
	}

	if (objectType.compare("MusicNote") == 0) {
		MusicNote* qb = new MusicNote();
		qb->SetPosition(x, y);
		//AddObject(qb);
		this->grid->Add(qb, cells);
	}

	if (objectType.compare("CoinBrick") == 0) {
		CoinBrick* qb = new CoinBrick();
		qb->SetPosition(x, y);
		//AddObject(qb);
		this->grid->Add(qb, cells);
	}

	if (objectType.compare("StrawCube") == 0) {
		StrawCube* qb = new StrawCube(this);
		qb->SetReward(LEAF_PRIZE);
		qb->SetPosition(x, y);
		//AddObject(qb);
		this->grid->Add(qb, cells);
	}

	if (objectType.compare("QuestionBlock") == 0) {
		QuestionBlock* qb = new QuestionBlock(this);
		qb->SetReward(COIN_PRIZE);
		qb->SetPosition(x, y);
		//AddObject(qb);
		this->grid->Add(qb, cells);
	}

	if (objectType.compare("RaccoonLeaf") == 0) {
		QuestionBlock* qb = new QuestionBlock(this);
		qb->SetReward(LEAF_PRIZE);
		qb->SetPosition(x, y);
		//AddObject(qb);
		this->grid->Add(qb, cells);
	}

	if (objectType.compare("GreenMushroom") == 0) {
		QuestionBlock* qb = new QuestionBlock(this);
		qb->SetReward(GMUSH_PRIZE);
		qb->SetPosition(x, y);
		//AddObject(qb);
		this->grid->Add(qb, cells);
	}

	if (objectType.compare("RedMushroom") == 0) {
		QuestionBlock* qb = new QuestionBlock(this);
		qb->SetReward(RMUSH_PRIZE);
		qb->SetPosition(x, y);
		//AddObject(qb);
		this->grid->Add(qb, cells);
	}
	
	if (objectType.compare("RedGoomba") == 0) {
		RedGoomba* rg = new RedGoomba();
		rg->SetPosition(x, y);
		//AddObject(rg);
		this->grid->Add(rg, cells);
	}

	if (objectType.compare("RedKoopas") == 0) {
		CKoopas* kp = new CKoopas(new NormalKoopas());
		kp->SetObjectState(new RedNormalKoopas(kp));
		kp->SetPosition(x, y);
		//AddObject(kp);
		this->grid->Add(kp, cells);
	}

	if (objectType.compare("Koopas") == 0) {
		CKoopas* kp = new CKoopas(new NormalKoopas);
		kp->SetPosition(x, y);
		//AddObject(kp);
		this->grid->Add(kp, cells);
	}

	if (objectType.compare("KoopasJumping") == 0) {
		CKoopas* kp = new CKoopas();
		kp->SetPosition(x, y);
		//AddObject(kp);
		this->grid->Add(kp, cells);
	}

	if (objectType.compare("Coin") == 0) {
		Coin* kp = new Coin();
		kp->SetPosition(x, y);
		//AddObject(kp);
		this->grid->Add(kp, cells);
	}

	if (objectType.compare("EndmapReward") == 0) {
		EndCard* kp = new EndCard();
		kp->SetPosition(x, y);
		//AddObject(kp);
		this->grid->Add(kp, cells);
	}

}

void CPlayScene::LoadSpecialObject(string objectType, vector< D3DXVECTOR2> cells, float xPos, float yPos, float width, float height, int length, int type, int direction,
	float desX, float desY, int moveDir, int targetReg)
{
	if (objectType.compare("Pipe") == 0) {
		Pipe* pipe = new Pipe(length, type, width, height, direction);
		pipe->SetPosition(xPos, yPos);
		//AddObject(pipe);
		this->grid->Add(pipe, cells);
	}
	
	if(objectType.compare("BeginPortal") == 0){
		BeginPortal* port = new BeginPortal(desX, desY, targetReg, moveDir);
		port->SetPosition(xPos, yPos);
		port->SetSize(width, height);
		//AddObject(port);
		this->grid->Add(port, cells);
		return;
	}

	if (objectType.compare("CloudPortal") == 0) {
		BeginPortal* port = new BeginPortal(desX, desY, targetReg, moveDir);
		port->SetPosition(xPos, yPos);
		port->SetSize(width, height);
		port->SetCloudPortal(1);
		//AddObject(port);
		this->grid->Add(port, cells);
		return;
	}

	if (objectType.compare("EndPortal") == 0) {
		EndPortal* pp = new EndPortal(moveDir);
		pp->SetPosition(xPos, yPos);
		pp->SetSize(width, height);
		//AddObject(pp);
		this->grid->Add(pp, cells);
		return;
	}
	
}

void CPlayScene::LoadGrid(int col, int row, int cellW, int cellH)
{
	this->grid = new Grid(col, row, cellW, cellH);
	DebugOut(L"Grid loaded!\n");
}
