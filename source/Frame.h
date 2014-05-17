#pragma once
#include "../SGD Wrappers/SGD_Geometry.h"
#include <string>

class Frame
{
public:
	Frame();
	~Frame();

private:
	SGD::Rectangle collisionREct;
};

