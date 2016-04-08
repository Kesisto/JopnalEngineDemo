#include <Jopnal/Jopnal.hpp>

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
};

#endif