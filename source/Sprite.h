#pragma once
#include <vector>
#include <map>
#include <string>
#include "../SGD Wrappers/SGD_Handle.h"
#include "Frame.h"
#include "AnimationManager.h"

class Frame;

class Sprite
{
public:

	Sprite();
	virtual ~Sprite();
	Sprite(const Sprite& cpy);
	Sprite& operator= (const Sprite& assign);

	/**********************************************************/
	// Accessor
	SGD::HTexture GetImage( void ) { return m_htImage; }
	std::string GetSpriteID( void ) { return m_strSpriteID; }
	bool IsLooping( void ) { return m_bIsLooping; }
	//Frame& GetFrame(int frameNum) { return *m_vFrames[frameNum]; }
	Frame GetFrame(int frameNum) { return *m_vFrames[frameNum]; }
	int GetFrameSize( void ) { return m_vFrames.size(); }
	int GetCurrFrame( void ) { return m_nCurrFrame; }
	void NextFrame( void );

	/**********************************************************/
	// Mutator
	void SetImage(const char * fileName);
	void SetSpriteID(std::string newID) { m_strSpriteID = newID; }
	void SetLooping(bool cont) { m_bIsLooping = cont; }

	/**********************************************************/
	// Interface
	void AddFrame(Frame* f) { m_vFrames.push_back(f); }

	/**********************************************************/
	// Data Members

private:
	/**********************************************************/
	// Data Members
	std::vector<Frame*> m_vFrames;
	SGD::HTexture m_htImage = SGD::INVALID_HANDLE;
	std::string m_strSpriteID;
	bool m_bIsLooping;
	int m_nCurrFrame = 0;
};

