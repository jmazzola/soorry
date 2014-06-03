/***************************************************************
|	File:		Button.cpp
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This class will handle, draw and monitor buttons
***************************************************************/

#include "Button.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "BitmapFont.h"

#include <cassert>

/**************************************************************/
// Accessors

SGD::HTexture Button::GetImage() { return m_hImage; }
string Button::GetText() { return m_szText; }
SGD::Point Button::GetPosition() { return m_ptPosition; }
SGD::Color Button::GetColor() { return m_cColor; }
SGD::Size Button::GetScale() { return m_sizeScale; }
SGD::Size Button::GetSize() { return m_sizeImageSize; }

// Mutators
void Button::SetText(string text)
{
	m_szText = text;
}

void Button::SetColor(SGD::Color color)
{
	m_cColor = color;
}

void Button::SetPosition(SGD::Point position)
{
	m_ptPosition = position;
}

void Button::SetScale(SGD::Size scale)
{
	m_sizeScale = scale;
}

void Button::SetSize(SGD::Size size)
{
	m_sizeImageSize = size;
}

/**************************************************************/
// Initialize
//	- configure the button's image
// [in] fileName - File path for the button's background image
// [in] pFont - A pointer of the bitmap font being used
void Button::Initialize(string fileName, BitmapFont* pFont)
{
	// Load the button's image
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(fileName.c_str());
	// Load the button's font
	m_pFont = pFont;
}

/**************************************************************/
// Terminate
//	- clean up resources for the button
void Button::Terminate(void)
{
	// Unload the button's image
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

/**************************************************************/
// Draw
// - render the button's image
// [in] text - the button's text
// [in] position - the x and y of the button being drawn
// [in] color - the color of the button's text
// [in] scale - the scale (x and y) of the button
// [in] rotation - the rotation of the button
void Button::Draw(string text, SGD::Point position, SGD::Color color, SGD::Size scale, float rotation)
{
	// Validate the image
	assert(m_hImage != SGD::INVALID_HANDLE
		&& "Button::Draw - image was not loaded");

	// Validate the parameter
	assert(text.c_str() != nullptr
		&& "Button::Draw - string cannot be null");

	// Draw the button
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(
		m_hImage, position, { 0, 0, m_sizeImageSize.width, m_sizeImageSize.height });
	
	// Draw the text in the middle of the button
	m_pFont->Draw(text,
		((int)position.x + (int)m_sizeImageSize.width / 2) - ((int)(m_pFont->GetTextWidth(text) * .5f * scale.width)),
		((int)position.y + (int)m_sizeImageSize.height / 2) - (int)(25 * scale.height),
		scale.width, color);
}