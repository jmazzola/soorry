#pragma once
#include "AnimationManager.h"
#include <string>

struct AnimationID
{
	std::string PlayerRun;
	std::string beaver;

	void LoadPlayerRun(std::string& id)
	{
		id = AnimationManager::GetInstance()->LoadSprites("resource/animation/piggy3.xml");
	}

	void LoadBeaver(std::string& id)
	{
		id = AnimationManager::GetInstance()->LoadSprites("resource/animation/beaver.xml");
	}
};