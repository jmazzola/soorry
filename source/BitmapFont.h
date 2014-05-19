/***************************************************************
|	File:		BitmapFont.cpp
|	Author:		Justin Patterson, Justin Mazzola
|	Course:		SGP
|	Purpose:	BitmapFont class draws text using an image
|				of fixed-size character glyphs and xml file holding
|				character info
***************************************************************/
#pragma once

#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Color.h"

#include <string>

using namespace std;


/**************************************************************/
// BitmapFont class
//	- displays text using an image of variable-width characters
//	- image MUST be in ASCII order!
//	- image can be missing characters, as long as there is space reserved
//	- image MUST have a size power-of-2 (e.g. 64, 128, 256, 512)

class BitmapFont
{
public:
	/**********************************************************/
	// Constructor & Destructor
	BitmapFont( void )	= default;
	~BitmapFont( void )	= default;

	
	/**********************************************************/
	// Initialize & Terminate
	void Initialize(string picFileName, string xmlFileName);
	void Terminate ( void );

	
	/**********************************************************/
	// Draw
	void Draw( string output, int x, int y,
			   float scale, SGD::Color color ) const;


private:
	/**********************************************************/
	// image

	SGD::HTexture m_hImage;

	// character data
	int m_nCharCount;

	int	m_nCharWidth[255];
	int	m_nCharHeight[255];
	int m_nCharX[255];
	int m_nCharY[255];
	int m_nCharXOffset[255];
	int m_nCharYOffset[255];
	int m_nXAdvance[255];

};
