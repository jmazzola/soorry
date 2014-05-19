/***************************************************************
|	File:		BitmapFont.cpp
|	Author:		Justin Patterson, Justin Mazzola
|	Course:		SGP
|	Purpose:	BitmapFont class draws text using an image 
|				of fixed-size character glyphs and xml file holding
|				character info
***************************************************************/

#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../TinyXML/tinyxml.h"

#include <ctype.h>
#include <cassert>


/**************************************************************/
// Initialize
//	- configure the font for "SGD_Font_Glow.png"
//	- probably should have parameters / config file
//
// [in] fileName - File path for the bitmap file image
// [in] firstChar - First character in the file

void BitmapFont::Initialize(const wchar_t* picFileName, string xmlFileName)
{
	// Load the image
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(picFileName);

	// Set characteristics
	m_nCharCount = 0;
	m_nCharWidth[255] = {};
	m_nCharHeight[255] = {};
	m_nCharX[255] = {};
	m_nCharY[255] = {};
	m_nCharXOffset[255] = {};
	m_nCharYOffset[255] = {};
	m_nCharXOffset[255] = {};

	// Make a XMLDocument for the font data
	TiXmlDocument doc;

	// Attempt to load it, if it doesn't work, gtfo.
	if (!doc.LoadFile(xmlFileName.c_str()))
		return;

	// Access the 'root' TinyXML Element
	TiXmlElement* pRoot = doc.RootElement();

	// Make sure the root is there
	if (pRoot == nullptr)
		return;

	// Grab the character data
	TiXmlElement* pChars = pRoot->FirstChildElement("chars");

	// Get the number of characters
	pChars->Attribute("count", &m_nCharCount);



}


/**************************************************************/
// Terminate
//	- clean up resources
void BitmapFont::Terminate( void )
{
	// Unload the image
	SGD::GraphicsManager::GetInstance()->UnloadTexture( m_hImage );
}


/**************************************************************/
// Draw
//	- draw the text one character at a time,
//	  using the Cell Algorithm to calculate the source rect
void BitmapFont::Draw( const char* output, int x, int y,
	float scale, SGD::Color color) const
{
	// Validate the image
	assert( m_hImage != SGD::INVALID_HANDLE 
			&& "BitmapFont::Draw - image was not loaded" );

	// Validate the parameter
	assert( output != nullptr 
			&& "BitmapFont::Draw - string cannot be null" );


	// Store the starting X position for newlines
	int colX = x;
}
