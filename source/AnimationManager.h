#pragma once
#include <string>
#include <vector>

class Sprite;
class AnimationTimestamp;

class AnimationManager
{
public:
	AnimationManager() = default;
	~AnimationManager() = default;
	/**********************************************************/
	// Interface
	bool LoadSprites(std::string fileName);
	void Update(AnimationTimestamp& ants, float dt);
	void Render(AnimationTimestamp& ants, int x, int y);

	/**********************************************************/
	// Accessors
	Sprite GetSprite(std::string id);

private:
	/**********************************************************/
	// Data Members
	std::vector<Sprite> m_vSprites;
};

