#include <Scenes/Level.hpp>
Level::Level() :jop::Scene("Level"), m_sine(0.f)
{
    getWorld().setDebugMode(true);

    ///Ligth
    createChild("Sun")->createComponent<jop::LightSource>(getRenderer(), jop::LightSource::Type::Point).setAttenuation(300).setCastShadows(true);
    findChild("Sun")->setPosition(0.f, 20.f, -7.f);
    findChild("Sun")->getComponent<jop::LightSource>()->setRenderMask(0x00000005);

    ///Skybox
    createChild("Skbo")->createComponent<jop::SkyBox>(getRenderer()).setMap(jop::ResourceManager::getResource<jop::Cubemap>(
        "Level/SboxRight.png", "Level/SboxLeft.png",
        "Level/SboxUp.png", "Level/SboxDown.png",
        "Level/SboxBack.png", "Level/SboxFront.png"));

    ///Ground
    jop::Material& ground = jop::ResourceManager::getEmptyResource<jop::Material>("defmat");
    ground.setMap(jop::Material::Map::Diffuse, jop::ResourceManager::getResource<jop::Texture2D>("Level/Gravel.jpg"));
    ground.setShininess(64.f);
    ground.setReflection(jop::Material::Reflection::Emission, jop::Color::Black);

    createChild("groundBox")->createComponent<jop::GenericDrawable>(getRenderer())
        .setModel(jop::Model(jop::Mesh::getDefault(), ground));
    findChild("groundBox")->setPosition(0.f, -7.f, -0.f)
    .setScale(40.f,1.f,40.f);  
   
    //Physics
    auto sens = createChild("sensor");
    sens->setPosition(-5.f, -3.f, -8);
    sens->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::BoxShape>("PhysSen", 1.f), jop::RigidBody::Type::StaticSensor));

    auto attribs = jop::Material::Attribute::DefaultLighting | jop::Material::Attribute::SpecularMap | jop::Material::Attribute::EmissionMap | jop::Material::Attribute::DiffuseMap;
    createChild("pln")->setPosition(0.f,
        findChild("groundBox")->getPosition().y,
        0.f);
    findChild("pln")->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::InfinitePlaneShape>("bigbcoll")));
    jop::Material& def = jop::ResourceManager::getEmptyResource<jop::Material>("defmat", attribs);

    ///Player
    createChild("Cam");
    findChild("Cam")->createComponent<jop::Camera>(getRenderer(), jop::Camera::Projection::Perspective);
    findChild("Cam")->createComponent<jop::Listener>();
    cam_speed = 3.f;

    createChild("Player")->
        createComponent<jop::GenericDrawable>(getRenderer())
        .setModel(jop::Model(jop::Mesh::getDefault()));
    findChild("Player")->setPosition(0.f, -5.f, -1.f).setScale(0.5f,1.f,0.5f);
    findChild("Player")->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::BoxShape>("boxcoll", 1.f), jop::RigidBody::Type::Dynamic, 1.f));
    findChild("Player")->getComponent<jop::GenericDrawable>()->setRenderGroup(2).setReceiveLights(true).castShadows();

    ///Envitorement
    createChild("Plank")->
    createComponent<jop::GenericDrawable>(getRenderer())
    .setModel(jop::Model(jop::Mesh::getDefault()));
    findChild("Plank")->setPosition(0.f, -5.f, -5.f).setScale(0.5f, 0.05f, 3.f);
    findChild("Plank")->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::BoxShape>("boxcoll", 1.f), jop::RigidBody::Type::Dynamic, 1.f));

}

Level::~Level()
{
}

void Level::preUpdate(const float dt)
{
}

void Level::postUpdate(const float dt)
{
    auto& Camera = findChild("Cam");
    auto& P1 = findChild("Player");
    ///INPUT
    auto& h = *jop::Engine::getSubsystem<jop::Window>()->getEventHandler();
    using jop::Keyboard;
        if (h.keyDown(Keyboard::LShift))
        {
                cam_sprint = 2.f;
        }
        if (!h.keyDown(Keyboard::LShift))
        {
                cam_sprint = 1.f;
        }
        if (h.keyDown(Keyboard::A) || h.keyDown(Keyboard::D))
        {
            auto& right = Camera->getGlobalRight();
            right.y = 0.f;
            P1->move((h.keyDown(Keyboard::D) ? 1.f : -1.f) * dt * (cam_speed*cam_sprint) * right);
        }
        if (h.keyDown(Keyboard::W) || h.keyDown(Keyboard::S))
        {
            auto& front = Camera->getGlobalFront();
            front.y = 0.f;
            P1->move((h.keyDown(Keyboard::W) ? 1.f : -1.f) * dt * (cam_speed*cam_sprint) * front);
        }
        Camera->setPosition(P1->getGlobalPosition().x, P1->getGlobalPosition().y + 2.f, P1->getGlobalPosition().z);
        if (h.keyDown(Keyboard::E))
        {
            auto& Plank = findChild("Plank");
            glm::vec3 difference = (Camera->getGlobalPosition() - (Plank->getPosition().x, Plank->getPosition().y, Plank->getPosition().z));
            float distance = sqrt(difference.x*difference.x + difference.y*difference.y + difference.z*difference.z);
            
            glm::mat4 invPanthom = Camera->getInverseMatrix();
            glm::vec3 up = { 0.f, 1.f, 0.f };
            invPanthom = glm::lookAt(Camera->getGlobalPosition(), Plank->getGlobalPosition(), up);
            glm::mat4 panthom = glm::inverse(invPanthom);           
            glm::vec3 front = glm::normalize(-glm::vec3(panthom[2][0], panthom[2][1], panthom[2][2]));
            float direction = sqrt(Camera->getGlobalFront().x*front.x + Camera->getGlobalFront().y*front.y + Camera->getGlobalFront().z*front.z);

            JOP_DEBUG_INFO("dis "<<distance<<" direc "<<direction);
            
            if (distance <= 5.5f&&direction > 0.99f)
                Plank->setPosition(0.f, 0.f, 0.f);

        }
        
  
}