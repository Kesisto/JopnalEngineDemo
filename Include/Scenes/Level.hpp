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
	std::vector<jop::WeakReference<jop::Object>>meteorites;
    std::vector<jop::WeakReference<jop::Object>>::iterator prop;
	jop::WeakReference<jop::Object> meteorite;
    jop::WeakReference<jop::Object> gun;
	jop::WeakReference<jop::Object> hammer;
	jop::WeakReference<jop::Object> shovel;
	jop::WeakReference<jop::Object> timer;
    bool objectPicked = false;
	bool holdRotation = false;
	int currentTool = 0;
	bool animate = false;
	float handPos = 2.5f;
	glm::vec3 animVec;
	glm::vec2 cPos;
	glm::quat cRot;
	float second;
	float meteoriteSpeed;
	int timeleft = 60;
    float cam_speed;
    float cam_sprint;
};

#endif