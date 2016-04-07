#include <Scenes/Splash.hpp>

Splash::Splash():jop::Scene("Splash"), m_sine(0.f)
{
        getWorld().setDebugMode(true);

        auto attribs = jop::Material::Attribute::DefaultLighting | jop::Material::Attribute::SpecularMap | jop::Material::Attribute::EmissionMap | jop::Material::Attribute::DiffuseMap;

        jop::Engine::setDeltaScale(2.f);
        setDeltaScale(.5f);


            createChild("envmap")->setPosition(-0.f, 0, -5);
            auto& record = findChild("envmap")->createComponent<jop::EnvironmentRecorder>(getRenderer());

            jop::Material& envMat = jop::ResourceManager::getEmptyResource<jop::Material>("envMat",
                jop::Material::Attribute::EnvironmentMap |
                jop::Material::Attribute::DefaultLighting);

            envMat.setMap(jop::Material::Map::Environment, *record.getTexture());
            envMat.setReflectivity(1.f)
                .setReflection(jop::Material::Reflection::Diffuse, jop::Color::Black)
                .setShininess(512.f)
                .removeAttributes(jop::Material::Attribute::AmbientConstant);

        jop::Material& def = jop::ResourceManager::getEmptyResource<jop::Material>("defmat", attribs);
        def.setMap(jop::Material::Map::Diffuse, jop::ResourceManager::getResource<jop::Texture2D>("Splash/JopnalLogo.png"));
        def.setMap(jop::Material::Map::Specular, jop::ResourceManager::getResource<jop::Texture2D>("container2_specular.png"));
        def.setMap(jop::Material::Map::Emission, jop::ResourceManager::getResource<jop::Texture2D>("matrix.jpg"));
        def.setShininess(64.f);
        def.setReflection(jop::Material::Reflection::Emission, jop::Color::Black);

        auto obj = createChild("Def");
        obj->createComponent<jop::GenericDrawable>(getRenderer())
            .setModel(jop::Model(jop::Mesh::getDefault(), def));
        obj->setPosition(0.f, 0.f, -5).rotate(0.f, 0.f, 3.1415926536f);

        createChild("SpotLight")->createComponent<jop::LightSource>(getRenderer(), jop::LightSource::Type::Spot).setAttenuation(jop::LightSource::AttenuationPreset::_320).setCutoff(glm::radians(10.f), glm::radians(20.f)).setCastShadows(true);
        findChild("SpotLight")->rotate(0, glm::radians(5.f), 0).setPosition(0.25f, -0.2f, -1.f);

        createChild("Cam");
        findChild("Cam")->createComponent<jop::Camera>(getRenderer(), jop::Camera::Projection::Perspective);
        findChild("Cam")->createComponent<jop::Listener>();


            auto attr = jop::Material::Attribute::AmbientConstant
                | jop::Material::Attribute::DiffuseMap
                | jop::Material::Attribute::Phong;

            auto ground = createChild("grnd");
            auto& comp = ground->createComponent<jop::GenericDrawable>(getRenderer());
            comp.setModel(jop::Model(jop::ResourceManager::getNamedResource<jop::BoxMesh>("rectasdf", 10.f, true), jop::ResourceManager::getEmptyResource<jop::Material>("grndmat", attr).setMap(jop::Material::Map::Diffuse, jop::ResourceManager::getResource<jop::Texture2D>("Black.png")).setReflection(jop::Color::Black, jop::Color::Black, jop::Color::Black, jop::Color::Black)));

            comp.setReceiveShadows(true);

            ground->setPosition(-0.f, -0.f, -5.f);
        
}

Splash::~Splash()
{}

void Splash::preUpdate(const float dt)
{
}

void Splash::postUpdate(const float dt)
{
  //  if (findChild("Def")->getPosition().z < -4.f)
  //      findChild("Def")->setPosition(0.f, -20.f, findChild("Def")->getPosition().z + 1.f);
}