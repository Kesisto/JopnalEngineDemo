#include <Jopnal/Jopnal.hpp>
#ifndef LEVEL_HPP
#define LEVEL_HPP
class Level :public jop::Scene
{
private:

    JOP_DISALLOW_COPY_MOVE(Level);

public:
    float m_sine;

    Level();
    ~Level();
    void preUpdate(const float dt) override;
    void postUpdate(const float dt) override;

private:
    std::vector<jop::WeakReference<jop::Object>>props;
    std::vector<jop::WeakReference<jop::Object>>::iterator prop;
    jop::WeakReference<jop::Object> gun;
    bool objectPicked = false;
    float cam_speed;
    float cam_sprint;
};

#endif