#pragma once
#include "sfml-engine/cameranode.h"

class FollowCameraNode : public gbh::CameraNode
{
public:
    void setTarget(std::shared_ptr<gbh::Node> target);
    
protected:
    virtual void onUpdate(double deltaTime) override;
    
private:
    std::shared_ptr<gbh::Node> m_target;
};

