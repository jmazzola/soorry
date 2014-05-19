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

void BitmapFont::Initialize(string picFileName, string xmlFileName)
{
	// Load the image
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(picFileName.c_str());

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

	// Get the number of characters in the font
	pChars->Attribute("count", &m_nCharCount);

	// Grab the 'char' element
	TiXmlElement* pChar = pChars->FirstChildElement("char");
	
	// Loop through all the characters
	for (int i = 0; i < m_nCharCount; i++)
	{
		// Grab what ASCII character we're on.
		int nChar = 0;
		pChar->Attribute("id", &nChar);

		// Grab the X position on the image
		pChar->Attribute("x", &m_nCharX[nChar]);
		// Grab the Y position on the image
		pChar->Attribute("y", &m_nCharY[nChar]);
		// Grab the character width of the character
		pChar->Attribute("width", &m_nCharWidth[nChar]);
		// Grab the character height of the character
		pChar->Attribute("height", &m_nCharHeight[nChar]);
		// Grab the X offset of the character
		pChar->Attribute("xoffset", &m_nCharXOffset[nChar]);
		// Grab the Y offset of the character
		pChar->Attribute("yoffset", &m_nCharYOffset[nChar]);
		// Grab how much you need to advance for the next character
		pChar->Attribute("xadvance", &m_nXAdvance[nChar]);

		// Move down to the next element of 'char'
		pChar = pChar->NextSiblingElement("char");
	}
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
//	- draw the text one character at a time
void BitmapFont::Draw( string output, int x, int y,
	float scale, SGD::Color color) const
{
	// Validate the image
	assert( m_hImage != SGD::INVALID_HANDLE
			&& "BitmapFont::Draw - image was not loaded" );

	// Validate the parameter
	assert( output.c_str() != nullptr
			&& "BitmapFont::Draw - string cannot be null" );


	// Store the starting X position for newlines
	int colX = x;

	// Iterate through each character in the output
	for (int i = 0; output.c_str()[i]; i++)
	{
		// Get the current character
		char curChar = output[i];

		// Check for space
		if (curChar == ' ')
		{
			// Move to the next position
			x += (int)(m_nCharWidth[curChar] * scale);
			continue;
		}

		// Check for newline
		else if (curChar == '\n')
		{
			// Move to the next row
			y += (int)(m_nCharHeight[curChar] * scale);
			x = colX;
			continue;
		}

		// Check for tab
		else if (curChar == '\t')
		{
			// Calculate the number of pixels from the start
			int pixels = x - colX;

			// Calculate the number of characters from the start
			int diff = pixels / (int)(m_nCharWidth[curChar] * scale);

			// Calculate the number of characters to add
			// to get a 4-space alignment
			int spaces = 4 - (diff % 4);

			// Move to the next position
			x += spaces * (int)(m_nCharWidth[curChar] * scale);
			continue;
		}

		// Calculate the source rect for the character
		SGD::Rectangle characterRect;
		characterRect.left = (float)(m_nCharX[curChar]);
		characterRect.top = (float)(m_nCharY[curChar]);
		characterRect.right = characterRect.left + m_nCharWidth[curChar];
		characterRect.bottom = characterRect.top + m_nCharHeight[curChar];

		// Draw the character
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_hImage, 
			{ (float)x + m_nCharXOffset[curChar] * scale, (float)y + m_nCharYOffset[curChar] * scale }, 
			characterRect, 0.0f, {}, color, { scale, scale });

		// Move to the next position
		x += (int)(m_nXAdvance[curChar] * scale);
	}
}