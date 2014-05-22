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

	static AnimationManager* GetInstance();
	static void DeleteInstance();

	/**********************************************************/
	// Interface
	std::string LoadSprites(std::string fileName);
	void Update(AnimationTimestamp& ants, float dt);
	void Render(AnimationTimestamp& ants, float x, float y);
	void UnloadSprites();
	void LoadAll();

	/**********************************************************/
	// Accessor
	AnimationTimestamp GetTimeStamp();
	Sprite* GetSprite(std::string nameID);

	/**********************************************************/
	// Data Members
	AnimationTimestamp m_sAnimationTS;

protected:
	/**********************************************************/
	// Data Members
	std::map<std::string, Sprite*> m_mSprites;
	std::vector<std::string> m_vSpriteNames;
	//std::vector<Sprite*> m_vSprites;
	static AnimationManager*	s_pInstance;
private:
	AnimationManager() = default;
	virtual ~AnimationManager() = default;
	AnimationManager(const AnimationManager&) = delete;
	AnimationManager& operator= (const AnimationManager&) = delete;
	bool made = false;

};

