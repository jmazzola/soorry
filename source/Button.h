/***************************************************************
|	File:		Button.h
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This class will handle, draw and monitor buttons
***************************************************************/

#pragma once

#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_Geometry.h"

#include <string>
using namespace std;

// Forward declaration
class BitmapFont;

class Button
{

public:
	/**********************************************************/
	// Constructor & Destructor
	Button(void) = default;
	~Button(void) = default;

	/**********************************************************/
	// Initialize & Terminate
	void Initialize(string picFileName, BitmapFont* pFont);
	void Terminate(void);

	/**********************************************************/
	// Draw
	void Draw(string text, SGD::Point position, SGD::Color color, SGD::Size scale, float rotation);

	/**********************************************************/
	// Accessors
	SGD::HTexture GetImage();
	string GetText();
	SGD::Point GetPosition();
	SGD::Color GetColor();
	SGD::Size GetScale();
	SGD::Size GetSize();

	/**********************************************************/
	// Mutators
	void SetText(string text);
	void SetPosition(SGD::Point point);
	void SetColor(SGD::Color color);
	void SetScale(SGD::Size scale);
	void SetSize(SGD::Size size);

private:
	/**********************************************************/
	// image
	SGD::HTexture m_hImage;
	// font
	BitmapFont* m_pFont;
	// text
	string m_szText;
	// position
	SGD::Point m_ptPosition;
	// color
	SGD::Color m_cColor;
	// scale
	SGD::Size m_sizeScale;
	// image size
	SGD::Size m_sizeImageSize;
};

