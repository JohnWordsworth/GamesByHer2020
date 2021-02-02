#pragma once

#include "sfml-engine/cameranode.h"

class FollowCameraNode : public gbh::CameraNode
{
public:
    FollowCameraNode();
    virtual ~FollowCameraNode() { }
    
    void setTarget(std::shared_ptr<gbh::Node> target);
    
protected:
    void onUpdate(double deltaTime) override;

private:
    std::shared_ptr<gbh::Node> m_target;
};
