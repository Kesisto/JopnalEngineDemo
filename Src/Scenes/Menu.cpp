#include<Scenes/Menu.hpp>
Menu::Menu() :jop::Scene("Menu"), m_sine(0.f)
{

    createChild("Cam");
    findChild("Cam")->createComponent<jop::Camera>(getRenderer(), jop::Camera::Projection::Perspective);
    findChild("Cam")->createComponent<jop::Listener>();

    auto attribs = jop::Material::Attribute::DefaultLighting | jop::Material::Attribute::SpecularMap | jop::Material::Attribute::EmissionMap | jop::Material::Attribute::DiffuseMap;

    createChild("Ambient")->createComponent<jop::LightSource>(getRenderer(), jop::LightSource::Type::Directional).setCutoff(glm::radians(10.f), glm::radians(20.f)).setAttenuation(200).setCastShadows(true);
    findChild("Ambient")->rotate(0, glm::radians(5.f), 0).setPosition(0.f, -0.2f, -3.f);

    jop::Material& def = jop::ResourceManager::getEmptyResource<jop::Material>("MenuBG", attribs);
    def.setMap(jop::Material::Map::Diffuse, jop::ResourceManager::getResource<jop::Texture2D>("Menu/Tools.jpg", true));
    def.setShininess(64.f);
    def.setReflection(jop::Material::Reflection::Emission, jop::Color::Black);

    auto obj = createChild("BG");
    obj->createComponent<jop::GenericDrawable>(getRenderer())
        .setModel(jop::Model(jop::Mesh::getDefault(), def));
    obj->setPosition(0.f, 0.f, -10.f).rotate(0.f, 0.f, 3.1415926536f).setScale(18.f,12.f,1.f);

    obj = createChild("Start");
    obj->createComponent<jop::Text>(getRenderer()).setString("START").setColor(jop::Color::Red);
    obj->setPosition(-1.8f, 1.f, -8.f).setScale(20.f,20.f,1.f);

    obj = createChild("Instructions");
    obj->createComponent<jop::Text>(getRenderer()).setString("INSTRUCTIONS").setColor(jop::Color::Red);
    obj->setPosition(-3.6f, -1.f, -8.f).setScale(20.f, 20.f, 1.f);

    jop::Material& Cur = jop::ResourceManager::getEmptyResource<jop::Material>("Cursor");
    Cur.setMap(jop::Material::Map::Diffuse, jop::ResourceManager::getResource<jop::Texture2D>("models/Shovel/Shovel.png", true));
    Cur.setShininess(1.f);
    Cur.setReflection(jop::Material::Reflection::Emission, jop::Color::Black);

    auto mod = createChild("Shovel");
    mod->createComponent<jop::ModelLoader>().load("models/Shovel/Shovel.obj");
    mod->getComponent<jop::GenericDrawable>()->getModel().setMaterial(Cur);
    mod->scale(0.5f).setPosition(-5.f, 1.f, -8.f).setRotation(0.f,0.f,-1.55f);
  
}

Menu::~Menu()
{
}

void Menu::preUpdate(const float dt)
{
}

void Menu::postUpdate(const float dt)
{
    auto& shovel = findChild("Shovel");
    findChild("Cam")->setRotation(0.f, 0.f, 0.f);

    auto& h = *jop::Engine::getSubsystem<jop::Window>()->getEventHandler();
    using jop::Keyboard;

    if (h.keyDown(Keyboard::Up))
    {
        shovel->setPosition(-5.f, 1.f, -8.f);
    }
    if (h.keyDown(Keyboard::Down))
    {
        shovel->setPosition(-6.5f, -1.f, -8.f);
    }

}
