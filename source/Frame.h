#pragma once
#include "../SGD Wrappers/SGD_Geometry.h"
#include <string>

class Frame
{
public:
	Frame() = default;
	~Frame() = default;
	Frame(const Frame& cpy) = default;
	Frame& operator= (const Frame& assign) = default;


	/**********************************************************/
	// Accessors
	SGD::Rectangle GetCollisionRect() { return m_rCollisionRect; }
	SGD::Rectangle GetFrameRect() { return m_rFrameRect; }
	float GetDuration() { return m_fDuration; }
	SGD::Point GetAnchorPoint() { return m_pAnchorPoint; }
	std::string GetTriggerID() { return m_strTriggerID; }

	/**********************************************************/
	// Mutators
	void SetCollisionRect(SGD::Rectangle newRect) { m_rCollisionRect = newRect; }
	void SetFrameRect(SGD::Rectangle newRect) { m_rFrameRect = newRect; }
	void SetDuration(float newDur) { m_fDuration = newDur; }
	void SetAnchorPoint(SGD::Point newPoint) { m_pAnchorPoint = newPoint; }
	void SetTriggerID(std::string newTrig) { m_strTriggerID = newTrig; }

private:
	/**********************************************************/
	// Data Members
	SGD::Rectangle m_rCollisionRect;
	SGD::Rectangle m_rFrameRect;
	float m_fDuration;
	SGD::Point m_pAnchorPoint;
	std::string m_strTriggerID;
};

