#include "cameranode.h"

void FollowCameraNode::setTarget(std::shared_ptr<gbh::Node> target)
{
    m_target = target;
}

//In the algorithm below, we calculate the difference between the x position of the target node and the camera. If the two are far enough apart, we move the camera towards the target, using the distance between the two positions as the ‘speed per second’. i.e. The further away from the target the camera is, the faster the camera moves.
void FollowCameraNode::onUpdate(double deltaTime)
{
    if (m_target == nullptr)
    {
        return;
    }
    
    float xDifference = m_target->getPosition().x - this->getPosition().x;
    float yDifference = m_target->getPosition().y - this->getPosition().y;
    
    float xLimit = 250;
    float yLimit = 150;
    
    if (xDifference > xLimit)
    {
        move((float)deltaTime * 2.0f * (xDifference - xLimit), 0);
    }
    else if (xDifference < -xLimit)
    {
        move((float)deltaTime * 2.0f * (xDifference + xLimit), 0);
    }
    
    if (yDifference > yLimit)
    {
        move(0, (float)deltaTime * 2.0f * (yDifference - yLimit));
    }
    else if (yDifference < -yLimit)
    {
        move(0, (float)deltaTime * 2.0f * (yDifference + yLimit));
    }
}
