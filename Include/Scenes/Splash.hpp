#include <Jopnal/Jopnal.hpp>
#include <Scenes/Level.hpp>

#ifndef SPLASH_HPP
#define SPLASH_HPP

class Splash:public jop::Scene
{
private:

    JOP_DISALLOW_COPY_MOVE(Splash);

public:
    float m_sine;

    Splash();
    ~Splash();
    void preUpdate(const float dt) override;
    void postUpdate(const float dt) override;

private:
    bool once;
};

#endif