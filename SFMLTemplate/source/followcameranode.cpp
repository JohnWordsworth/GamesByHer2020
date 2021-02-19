#include "followcameranode.h"

#include <iostream>


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
    
    if (xDifference > m_xLimit)
    {
        move((float)deltaTime * m_followSpeed * (xDifference - m_xLimit), 0);
    }
    else if (xDifference < -m_xLimit)
    {
        move((float)deltaTime * m_followSpeed * (xDifference + m_xLimit), 0);
    }
    
    if (yDifference > m_yLimit)
    {
        move(0, (float)deltaTime * m_followSpeed * (yDifference - m_yLimit));
    }
    else if (yDifference < -m_yLimit)
    {
        move(0, (float)deltaTime * m_followSpeed * (yDifference + m_yLimit));
    }
}
