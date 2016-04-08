#include <Scenes/Level.hpp>
Level::Level() :jop::Scene("Level"), m_sine(0.f)
{
    ///Ligth
    createChild("Sun")->createComponent<jop::LightSource>(getRenderer(), jop::LightSource::Type::Point).setAttenuation(jop::LightSource::AttenuationPreset::_320).setCastShadows(true);
    findChild("Sun")->setPosition(0.f, 20.f, -7.f);
   
    ///Player
    createChild("Cam");
    findChild("Cam")->createComponent<jop::Camera>(getRenderer(), jop::Camera::Projection::Perspective);
    findChild("Cam")->createComponent<jop::Listener>();
    cam_speed = 3.f;
    cam_jump = 0.f;
    ///Skybox
    auto sky = createChild("grnd");
    auto& comp = sky->createComponent<jop::GenericDrawable>(getRenderer());
    comp.setModel(jop::Model(jop::ResourceManager::getNamedResource<jop::BoxMesh>("skybox", 40.f, true), jop::ResourceManager::getEmptyResource<jop::Material>("grndmat").setMap(jop::Material::Map::Diffuse, jop::ResourceManager::getResource<jop::Texture2D>("Level/SkyBox.bmp"))));
    comp.setReceiveShadows(true);
    sky->setPosition(-0.f, -0.f, -1.f);

    ///Ground
    jop::Material& ground = jop::ResourceManager::getEmptyResource<jop::Material>("defmat");
    ground.setMap(jop::Material::Map::Diffuse, jop::ResourceManager::getResource<jop::Texture2D>("Level/Gravel.jpg"));
    ground.setShininess(64.f);
    ground.setReflection(jop::Material::Reflection::Emission, jop::Color::Black);

    createChild("groundBox")->createComponent<jop::GenericDrawable>(getRenderer())
        .setModel(jop::Model(jop::Mesh::getDefault(), ground));
    findChild("groundBox")->setPosition(0.f, -7.f, -0.f)
    .setScale(40.f,1.f,40.f);
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
    if (Camera->getPosition().y > -5.f)
        Camera->move(-dt * 9.81f * Camera->getGlobalUp());

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
            Camera->move((h.keyDown(Keyboard::D) ? 1.f : -1.f) * dt * (cam_speed*cam_sprint) * right);
        }
        if (h.keyDown(Keyboard::W) || h.keyDown(Keyboard::S))
        {
            auto& front = Camera->getGlobalFront();
            front.y = 0.f;
            Camera->move((h.keyDown(Keyboard::W) ? 1.f : -1.f) * dt * (cam_speed*cam_sprint) * front);
        }


  
}