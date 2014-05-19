#pragma once
#include <string>
#include <vector>
#include <map>

class Sprite;

struct AnimationTimestamp
{
	/**********************************************************/
	// Data Members
	std::string m_nCurrAnimation;
	int m_nCurrFrame;
	float m_fTimeOnFrame;
};

class AnimationManager
{
public:
	AnimationManager() = default;
	~AnimationManager() = default;
	/**********************************************************/
	// Interface
	bool LoadSprites(std::string fileName);
	void Update(AnimationTimestamp& ants, float dt);
	void Render(AnimationTimestamp& ants, float x, float y);

	/**********************************************************/
	// Data Members
	AnimationTimestamp m_sAnimationTS;

private:
	/**********************************************************/
	// Data Members
	std::map<std::string, Sprite> m_mSprites;
};

