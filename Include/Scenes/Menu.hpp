#include <Jopnal/Jopnal.hpp>
#include <Scenes/Level.hpp>

#ifndef MENU_HPP
#define MENU_HPP

class Menu :public jop::Scene
{
private:

    JOP_DISALLOW_COPY_MOVE(Menu);

public:
    float m_sine;

    Menu();
    ~Menu();
    void preUpdate(const float dt) override;
    void postUpdate(const float dt) override;

private:

    jop::WeakReference<jop::Object> shovel;
    bool start=true;
};

#endif