#pragma once
#include "AnimationManager.h"
#include <string>

struct AnimationID
{
	std::string PlayerRun;

	void LoadPlayerRun(std::string& id)
	{
		id = AnimationManager::GetInstance()->LoadSprites("resource/animation/piggy3.xml");
	}
};