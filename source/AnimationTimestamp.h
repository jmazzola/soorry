#pragma once
class AnimationTimestamp
{
public:
	AnimationTimestamp();
	~AnimationTimestamp();

	/**********************************************************/
	// Accessors
	int GetCurrAnimation() { return m_nCurrAnimation; }
	int GetCurrFrame() { return m_nCurrFrame; }
	float GetTimeFrame() { return m_fTimeOnFrame; }

	/**********************************************************/
	// Mutator
	void SetCurrAnimation(int currA) { m_nCurrAnimation = currA; }
	void SetCurrFrame(int currF) { m_nCurrFrame = currF; }
	void SetTimeFrame(float newTF) { m_fTimeOnFrame = newTF; }

private:
	/**********************************************************/
	// Data Members
	int m_nCurrAnimation;
	int m_nCurrFrame;
	float m_fTimeOnFrame;
};

