#include "Sprite.h"
#include "Frame.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

Sprite::Sprite()
{
}


Sprite::~Sprite()
{
}

void Sprite::SetImage(const char * fileName)
{
	SGD::GraphicsManager::GetInstance()->LoadTexture(fileName);
}
