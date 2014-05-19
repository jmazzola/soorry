/***************************************************************
|	File:		BitmapFont.cpp
|	Author:		Justin Patterson
|	Course:		SGD 1403
|	Purpose:	BitmapFont class draws text using an image
|				of fixed-size character glyphs
***************************************************************/

#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <ctype.h>
#include <cassert>


/**************************************************************/
// Initialize
//	- configure the font for "SGD_Font_Glow.png"
//	- probably should have parameters / config file
//
// [in] fileName - File path for the bitmap file image
// [in] firstChar - First character in the file

void BitmapFont::Initialize()
{
	// Load the image
	/*m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
				L"resource/graphics/JMP_font.png" );*/

	// Set all the character widths to 0
	m_nCharWidth = 0;
	// Set all the character heights to 0
	m_nCharHeight = 0;
	
	m_nNumRows = 6;
	m_nNumCols = 10;
	
	m_cFirstChar		= ' ';
	m_bOnlyUppercase	= true;
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

	// Iterate through the characters in the string
	for( int i = 0; output[ i ]; i++ )
	{
		// Get the current character
		char ch = output[ i ];


		// Check for whitespace
		if( ch == ' ' )
		{
			// Move to the next position
			x += (int)(m_nCharWidth * scale);
			continue;
		}
		else if( ch == '\n' )
		{
			// Move to the next row
			y += (int)(m_nCharHeight * scale);
			x =  colX;
			continue;
		}
		else if( ch == '\t' )
		{
			// Calculate the number of pixels from the start
			int pixels = x - colX;

			// Calculate the number of characters from the start
			int diff = pixels / (int)(m_nCharWidth * scale);

			// Calculate the number of characters to add
			// to get a 4-space alignment
			int spaces = 4 - (diff%4);

			
			// Move to the next position
			x += spaces * (int)(m_nCharWidth * scale);
			continue;
		}


		// Convert to uppercase?
		if( m_bOnlyUppercase == true )
			ch = toupper( ch );


		// Calculate the tile ID for this character
		int id = ch - m_cFirstChar;

		// Calculate the source rect for that glyph
		SGD::Rectangle cell;
		cell.left	= (float)( (id % m_nNumCols) * m_nCharWidth  );
		cell.top	= (float)( (id / m_nNumCols) * m_nCharHeight );
		cell.right	= cell.left + m_nCharWidth;
		cell.bottom	= cell.top  + m_nCharHeight;

		// Draw the character
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_hImage, { (float)x, (float)y }, 
			cell, 0.0f, {}, 
			color, {scale, scale} );

		
		// Move to the next position
		x += (int)(m_nCharWidth * scale);
	}
}
