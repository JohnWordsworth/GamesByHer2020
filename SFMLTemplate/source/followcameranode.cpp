#include "followcameranode.h"

#include <iostream>

FollowCameraNode::FollowCameraNode()
{
    
}


void FollowCameraNode::setTarget(std::shared_ptr<gbh::Node> target)
{
    m_target = target;
}


void FollowCameraNode::onUpdate(double deltaTime)
{
    if (m_target == nullptr) {
        return;
    }

    float xDifference = m_target->getPosition().x - this->getPosition().x;
    float yDifference = m_target->getPosition().y - this->getPosition().y;

    if (xDifference > 300 || xDifference < -300) {
        float distance = (float)deltaTime * xDifference;
        move(distance, 0);
    }

    if (yDifference > 200 || yDifference < -200) {
        float distance = (float)deltaTime * yDifference;
        move(0, distance);
    }
}
