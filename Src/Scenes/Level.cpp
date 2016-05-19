#include <Scenes/Level.hpp>
Level::Level() :jop::Scene("Level"), m_sine(0.f)
{
    getWorld().setDebugMode(true);

    ///Ligth
    createChild("Sun")->createComponent<jop::LightSource>(getRenderer(), jop::LightSource::Type::Point).setAttenuation(300).setCastShadows(true);
    findChild("Sun")->setPosition(0.f, 20.f, -7.f);
    findChild("Sun")->getComponent<jop::LightSource>()->setRenderMask(0x00000005);

	jop::Material& ad = jop::ResourceManager::getEmptyResource<jop::Material>("mat");
	ad.setMap(jop::Material::Map::Diffuse, jop::ResourceManager::getResource<jop::Texture2D>("models/NailGun/nailgun.png", true));
	ad.setShininess(64.f);
	ad.setReflection(jop::Material::Reflection::Emission, jop::Color::Black);

	auto mod = createChild("gun");
	mod->createComponent<jop::ModelLoader>().load("models/NailGun/Nailgun.obj");
	mod->getComponent<jop::GenericDrawable>()->getModel().setMaterial(ad);
	mod->scale(0.25f).setPosition(-2.5f, -4.f, -4.f);

    ///Skybox
    createChild("Skbo")->createComponent<jop::SkyBox>(getRenderer()).setMap(jop::ResourceManager::getResource<jop::Cubemap>(
        "Level/skybox_right.jpg", "Level/skybox_left.jpg",
        "Level/skybox_top.jpg", "Level/skybox_bottom.jpg",
        "Level/skybox_back.jpg", "Level/skybox_front.jpg",true));

    ///Ground
    jop::Material& ground = jop::ResourceManager::getEmptyResource<jop::Material>("defmat");
    ground.setMap(jop::Material::Map::Diffuse, jop::ResourceManager::getResource<jop::Texture2D>("Level/Gravel.jpg",true));
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
        findChild("groundBox")->getLocalPosition().y,
        0.f);
    findChild("pln")->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::InfinitePlaneShape>("bigbcoll")));
    jop::Material& def = jop::ResourceManager::getEmptyResource<jop::Material>("defmat", attribs);

    ///Player
    createChild("Cam");
    findChild("Cam")->createComponent<jop::Camera>(getRenderer(), jop::Camera::Projection::Perspective);
    findChild("Cam")->createComponent<jop::Listener>();
	findChild("Cam")->setScale(0.1f);
    cam_speed = 5.f;

    createChild("Player")->
        createComponent<jop::GenericDrawable>(getRenderer())
        .setModel(jop::Model(jop::Mesh::getDefault()));
    findChild("Player")->setPosition(0.f, -5.f, -1.f).setScale(0.5f,0.5f,0.5f);
    findChild("Player")->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::SphereShape>("coll", 1.f), jop::RigidBody::Type::Dynamic, 1.f));
    findChild("Player")->getComponent<jop::GenericDrawable>()->setRenderGroup(2).setReceiveLights(true).castShadows();
    
    createChild("Hand")->
        createComponent<jop::GenericDrawable>(getRenderer())
        .setModel(jop::Model(jop::Mesh::getDefault()));
        findChild("Hand")->setPosition(findChild("Player")->getGlobalFront().x, findChild("Player")->getGlobalFront().y, findChild("Player")->getGlobalFront().z).setScale(0.2f, 0.2f, 0.2f);
        findChild("Hand")->getComponent<jop::GenericDrawable>()->setRenderGroup(3).setReceiveLights(false);

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
glm::vec3 lastpos;
glm::vec3 lastrot;
bool objectPicked = false;
void Level::postUpdate(const float dt)
{
    auto& Camera = findChild("Cam");
    auto& Hand = findChild("Hand");
    auto& P1 = findChild("Player");
    auto& P1body = *P1->getComponent<jop::RigidBody>();
	P1body.setAngularVelocity(glm::vec3(0.f,0.f,0.f));



    //Position updates
    Camera->setPosition(P1->getGlobalPosition().x, P1->getGlobalPosition().y + 2.f, P1->getGlobalPosition().z);
	Hand->setRotation(Camera->getLocalRotation());
	Hand->setPosition(Camera->getGlobalPosition());
	Hand->move(2.5f * Camera->getGlobalFront());

    lastrot = Camera->getGlobalFront();
    lastpos = Camera->getGlobalPosition();
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
        if (h.keyDown(Keyboard::Space))
        {
            glm::vec3 jump = { 0.f, 20.f, 0.f };
            P1body.applyForce(jump,P1->getGlobalUp());
        }
        if (h.keyDown(Keyboard::A) || h.keyDown(Keyboard::D))
        {
            auto& right = Camera->getGlobalRight();
            right.y = 0.f;
            P1body.applyCentralImpulse(((h.keyDown(Keyboard::D) ? 1.f : -1.f) * dt * (cam_speed*cam_sprint) * right));
        }
        if (h.keyDown(Keyboard::W) || h.keyDown(Keyboard::S))
        {
            auto& front = Camera->getGlobalFront();
            front.y = 0.f;
            P1body.applyCentralImpulse(((h.keyDown(Keyboard::W) ? 1.f : -1.f) * dt * (cam_speed*cam_sprint) * front));
        }
        if (!h.keyDown(Keyboard::W) && !h.keyDown(Keyboard::S) && !h.keyDown(Keyboard::A) && !h.keyDown(Keyboard::D))
        {
            glm::vec3 stop = { 0.f, P1body.getGravity().y, 0.f };
            P1body.setAngularVelocity(stop);
        }
        if (h.keyDown(Keyboard::E))
        {
            auto& Plank = findChild("Plank");
            glm::vec3 difference = (Camera->getGlobalPosition() - Plank->getGlobalPosition());
            float distance = sqrt(difference.x*difference.x + difference.y*difference.y + difference.z*difference.z);

            glm::vec3 up = { 0.f, 1.f, 0.f };
            glm::mat4 invPanthom = Camera->getInverseTransform().getMatrix();
            invPanthom = glm::lookAt(Camera->getGlobalPosition(), Plank->getGlobalPosition(), up);
            glm::mat4 panthom = glm::inverse(invPanthom);
            glm::vec3 front = glm::normalize(-glm::vec3(panthom[2][0], panthom[2][1], panthom[2][2]));
            float direction = sqrt(Camera->getGlobalFront().x*front.x + Camera->getGlobalFront().y*front.y + Camera->getGlobalFront().z*front.z);
            
            if (distance <= 7.f&&direction > 0.99f)
            {
                objectPicked = true;
            }

        }
        if (!h.keyDown(Keyboard::E))
        {
            objectPicked = false;
        }

         if (objectPicked)
         {
            auto& Plank = findChild("Plank");
            glm::vec3 difference = (Camera->getGlobalPosition() - Plank->getGlobalPosition());
            float hor = 0.f;
            float ver = 0.f;

            glm::vec3 force = { hor, 5.f*(Camera->getGlobalPosition().y - Plank->getGlobalPosition().y), ver };

			float distance = sqrt((Plank->getGlobalPosition().x - Hand->getGlobalPosition().x)*(Plank->getGlobalPosition().x - Hand->getGlobalPosition().x) 
				+ (Plank->getGlobalPosition().y - Hand->getGlobalPosition().y)*(Plank->getGlobalPosition().y - Hand->getGlobalPosition().y) 
				+ (Plank->getGlobalPosition().z - Hand->getGlobalPosition().z)*(Plank->getGlobalPosition().z - Hand->getGlobalPosition().z));
			
            if (Plank->getGlobalPosition().x != Hand->getGlobalPosition().x)
            {
				force.x = (Plank->getGlobalPosition().x < Hand->getGlobalPosition().x ? 1.f : -1.f)
					*distance*2.f;
            }
            if (Plank->getGlobalPosition().y != Camera->getGlobalPosition().y)
            {
                force.y = (Plank->getGlobalPosition().y < Hand->getGlobalPosition().y ? 1.f : 0.f)
					*distance*2.f;
            }
            if (Plank->getGlobalPosition().z != Camera->getGlobalPosition().z)
            {
                force.z = (Plank->getGlobalPosition().z < Hand->getGlobalPosition().z ? 1.f : -1.f)
					*distance*2.f;
            }
                Plank->getComponent<jop::RigidBody>()->setLinearVelocity(force);

				force.x = 0.f;
				force.y = 0.f;
				force.z = 0.f;
				if (h.keyDown(Keyboard::KeypadSeven))
				{
					force.z = -2.f;
				}
				if (h.keyDown(Keyboard::KeypadNine))
				{
					force.z = 2.f;
				}

				if (h.keyDown(Keyboard::KeypadFour))
				{
					force.y = -2.f;
				}
				if (h.keyDown(Keyboard::KeypadSix))
				{
					force.y = 2.f;
				}
				if (h.keyDown(Keyboard::KeypadEight))
				{
					force.x = -2.f;
				}
				if (h.keyDown(Keyboard::KeypadFive))
				{
					force.x = 2.f;
				}
				
				Plank->getComponent<jop::RigidBody>()->setAngularVelocity(force);

				if (h.mouseButtonDown(jop::Mouse::Left))
				{
					Plank->getComponent<jop::RigidBody>()->~RigidBody();
					findChild("Plank")->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::BoxShape>("boxcoll", 1.f), jop::RigidBody::Type::Static, 1.f));	
					Plank->getComponent<jop::RigidBody>()->setGravity(glm::vec3(0.f, 0.f, 0.f));
				}

        }
        
  
}