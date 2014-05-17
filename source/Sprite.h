#pragma once
#include <vector>
#include <map>
#include <string>
#include "../SGD Wrappers/SGD_Handle.h"


class Frame;

class Sprite
{
public:
	Sprite();
	~Sprite();

	/**********************************************************/
	// Accessor
	SGD::HTexture GetImage() { return m_htImage; }
	std::string GetSpriteID() { return m_strSpriteID; }
	bool IsLooping() { return m_bIsLooping; }
	Frame& GetFrame(int frameNum) { return m_vFrames[frameNum]; }

	/**********************************************************/
	// Mutator
	void SetImage(const char * fileName);
	void SetSpriteID(std::string newID) { m_strSpriteID = newID; }
	void SetLooping(bool cont) { m_bIsLooping = cont; }

private:
	/**********************************************************/
	// Data Members
	std::vector<Frame> m_vFrames;
	SGD::HTexture m_htImage;
	std::string m_strSpriteID;
	bool m_bIsLooping;
	std::map<std::string, Sprite> m_mFrames;
};

