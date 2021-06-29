#pragma once
#include "Utils.h"
class Effects;

class EffectVault
{
private:
	static EffectVault* __instance;

protected:
	vector <Effects*> effects;

public:
	EffectVault();

	void Update(DWORD dt);

	void Render();

	void AddEffect(Effects* effect);

	void ClearEffects();

	static EffectVault* GetInstance();

};

