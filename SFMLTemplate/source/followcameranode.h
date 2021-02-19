#pragma once

#include "sfml-engine/cameranode.h"

/**
   A specialized camera node which tracks a target node and moves in such a way
   as to try to keep that node with the central area of the viewport.
 */
class FollowCameraNode : public gbh::CameraNode
{
public:
	/** Set which node this camera should follow. */
    void setTarget(std::shared_ptr<gbh::Node> target);
    
	/** Set a speed multiplier for the camera. A good value for this is 2.0f */
	void setFollowSpeed(float value) { m_followSpeed = value; }
	float getFollowSpeed() const { return m_followSpeed; }

	/** How many units of distance away from the target node until this camera starts following. */
	void setXLimit(float value) { m_xLimit = value; }
	void setYLimit(float value) { m_yLimit = value; }
	float getXLimit() const { return m_xLimit; }
	float getYLimit() const { return m_yLimit; }
	
protected:
    void onUpdate(double deltaTime) override;

private:
	std::shared_ptr<gbh::Node> m_target;

	float m_followSpeed = 2.0f;
	float m_xLimit = 250.0f;
	float m_yLimit = 150.0f;
	
};
