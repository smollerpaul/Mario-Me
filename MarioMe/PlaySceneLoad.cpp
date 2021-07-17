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

void CPlayScene::LoadMapObjects(string objectType, float x, float y, float width, float height)
{
	if (objectType.compare("SpawnPoint") == 0) {
		player->SetPosition(x, y);
	}

	if (objectType.compare("Venus") == 0) {
		Venus* kp = new Venus(this);
		kp->SetPosition(x, y);
		AddObject(kp);
	}

	if (objectType.compare("RedVenus") == 0) {
		Venus* kp = new Venus(this);
		kp->SetPosition(x, y);
		kp->isRedVenus = 1;
		AddObject(kp);
	}

	if (objectType.compare("Piranha") == 0) {
		Piranha* kp = new Piranha();
		kp->SetPosition(x, y);
		AddObject(kp);
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

	if (objectType.compare("KoopasBound") == 0) {
		KoopasBound* kpB = new KoopasBound();
		kpB->SetPosition(x, y);
		kpB->SetSize(width, height);
		AddObject(kpB);
	}

	if (objectType.compare("BoomBound") == 0) {
		BoomBound* bbb = new BoomBound();
		bbb->SetPosition(x, y);
		bbb->SetSize(width, height);
		AddObject(bbb);
	}

	if (objectType.compare("Brick") == 0) {
		CBrick* bb = new CBrick();
		bb->SetPosition(x, y);
		AddObject(bb);
	}

	if (objectType.compare("Void") == 0) {
		Void* v = new Void();
		v->SetPosition(x, y);
		v->SetSize(width, height);
		AddObject(v);
	}

	if (objectType.compare("Goomba") == 0) {
		CGoomba* goomba = new CGoomba();
		goomba->SetPosition(x, y);
		AddObject(goomba);
	}

	if (objectType.compare("PSwitch") == 0) {
		QuestionBlock* qb = new QuestionBlock(this);
		qb->SetReward(PSWITCH_PRIZE);
		qb->SetAsBrick(1);
		qb->SetPosition(x, y);
		AddObject(qb);
	}

	if (objectType.compare("MusicNote") == 0) {
		MusicNote* qb = new MusicNote();
		qb->SetPosition(x, y);
		AddObject(qb);
	}

	if (objectType.compare("CoinBrick") == 0) {
		CoinBrick* qb = new CoinBrick();
		qb->SetPosition(x, y);
		AddObject(qb);
	}

	if (objectType.compare("StrawCube") == 0) {
		StrawCube* qb = new StrawCube(this);
		qb->SetReward(LEAF_PRIZE);
		qb->SetPosition(x, y);
		AddObject(qb);
	}

	if (objectType.compare("QuestionBlock") == 0) {
		QuestionBlock* qb = new QuestionBlock(this);
		qb->SetReward(COIN_PRIZE);
		qb->SetPosition(x, y);
		AddObject(qb);
	}

	if (objectType.compare("RaccoonLeaf") == 0) {
		QuestionBlock* qb = new QuestionBlock(this);
		qb->SetReward(LEAF_PRIZE);
		qb->SetPosition(x, y);
		AddObject(qb);
	}

	if (objectType.compare("GreenMushroom") == 0) {
		QuestionBlock* qb = new QuestionBlock(this);
		qb->SetReward(GMUSH_PRIZE);
		qb->SetPosition(x, y);
		AddObject(qb);
	}
	
	if (objectType.compare("RedGoomba") == 0) {
		RedGoomba* rg = new RedGoomba();
		rg->SetPosition(x, y);
		AddObject(rg);
	}

	if (objectType.compare("RedKoopas") == 0) {
		CKoopas* kp = new CKoopas(new NormalKoopas());
		kp->SetObjectState(new RedNormalKoopas(kp));
		kp->SetPosition(x, y);
		AddObject(kp);
	}

	if (objectType.compare("Koopas") == 0) {
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

	if (objectType.compare("EndmapReward") == 0) {
		EndCard* kp = new EndCard();
		kp->SetPosition(x, y);
		AddObject(kp);
	}

	//boombro

}

void CPlayScene::LoadSpecialObject(float xPos, float yPos, float width, float height, int length, int type, int direction)
{
	Pipe* pipe = new Pipe(length, type, width, height, direction);
	pipe->SetPosition(xPos, yPos);
	AddObject(pipe);
}
