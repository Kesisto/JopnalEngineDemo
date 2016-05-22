#include <Scenes/Level.hpp>
Level::Level() :jop::Scene("Level"), m_sine(0.f)
{
   // getWorld().setDebugMode(true);

    ///Ligth
    createChild("Sun")->createComponent<jop::LightSource>(getRenderer(), jop::LightSource::Type::Directional).setAttenuation(300).setCastShadows(true);
    findChild("Sun")->setPosition(0.f, 20.f, -7.f).setRotation(-1.f,0.f,0.f);
    findChild("Sun")->getComponent<jop::LightSource>()->setRenderMask(0x00000005);

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
    findChild("groundBox")->setPosition(0.f, -7.5f, -0.f)
    .setScale(40.f,1.f,40.f);  
	findChild("groundBox")->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::BoxShape>("groundCollision", findChild("groundBox")->getGlobalScale()), jop::RigidBody::Type::Static, 1.f));
	findChild("groundBox")->getComponent<jop::RigidBody>()->setGravity(glm::vec3(0.f, 0.f, 0.f));

	jop::Material& wallGrass = jop::ResourceManager::getEmptyResource<jop::Material>("wall");
	wallGrass.setMap(jop::Material::Map::Diffuse, jop::ResourceManager::getResource<jop::Texture2D>("Level/Grass.jpg", true));
	wallGrass.setShininess(64.f);
	wallGrass.setReflection(jop::Material::Reflection::Emission, jop::Color::Black);

	createChild("wallGrass")->createComponent<jop::GenericDrawable>(getRenderer())
		.setModel(jop::Model(jop::Mesh::getDefault(), wallGrass));
	findChild("wallGrass")->setPosition(40.f, -7.5f, -0.f)
		.setScale(40.f, 1.f, 40.f);

	createChild("wallGrass2")->createComponent<jop::GenericDrawable>(getRenderer())
		.setModel(jop::Model(jop::Mesh::getDefault(), wallGrass));
	findChild("wallGrass2")->setPosition(-40.f, -7.5f, -0.f)
		.setScale(40.f, 1.f, 40.f);

	createChild("wallGrass3")->createComponent<jop::GenericDrawable>(getRenderer())
		.setModel(jop::Model(jop::Mesh::getDefault(), wallGrass));
	findChild("wallGrass3")->setPosition(0.f, -7.5f, -40.f)
		.setScale(40.f, 1.f, 40.f);

	createChild("wallGrass4")->createComponent<jop::GenericDrawable>(getRenderer())
		.setModel(jop::Model(jop::Mesh::getDefault(), wallGrass));
	findChild("wallGrass4")->setPosition(0.f, -7.5f, 40.f)
		.setScale(40.f, 1.f, 40.f);

	createChild("wallGrass5")->createComponent<jop::GenericDrawable>(getRenderer())
		.setModel(jop::Model(jop::Mesh::getDefault(), wallGrass));
	findChild("wallGrass5")->setPosition(-40.f, -7.5f, 40.f)
		.setScale(40.f, 1.f, 40.f);

	createChild("wallGrass6")->createComponent<jop::GenericDrawable>(getRenderer())
		.setModel(jop::Model(jop::Mesh::getDefault(), wallGrass));
	findChild("wallGrass6")->setPosition(40.f, -7.5f, 40.f)
		.setScale(40.f, 1.f, 40.f);

	createChild("wallGrass7")->createComponent<jop::GenericDrawable>(getRenderer())
		.setModel(jop::Model(jop::Mesh::getDefault(), wallGrass));
	findChild("wallGrass7")->setPosition(40.f, -7.5f, -40.f)
		.setScale(40.f, 1.f, 40.f);

	createChild("wallGrass8")->createComponent<jop::GenericDrawable>(getRenderer())
		.setModel(jop::Model(jop::Mesh::getDefault(), wallGrass));
	findChild("wallGrass8")->setPosition(-40.f, -7.5f, -40.f)
		.setScale(40.f, 1.f, 40.f);

   
    //Physics
    auto sens = createChild("sensor");
    sens->setPosition(-5.f, -3.f, -8.f);
    sens->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::BoxShape>("PhysSen", 1.f), jop::RigidBody::Type::StaticSensor));

    auto attribs = jop::Material::Attribute::DefaultLighting | jop::Material::Attribute::SpecularMap | jop::Material::Attribute::EmissionMap | jop::Material::Attribute::DiffuseMap;
    createChild("pln")->setPosition(0.f,-15.f,0.f);
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
    findChild("Player")->setPosition(0.f, -3.f, -1.f).setScale(0.5f,0.5f,0.5f);
    findChild("Player")->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::SphereShape>("coll", 1.f), jop::RigidBody::Type::Dynamic, 1.f));
    findChild("Player")->getComponent<jop::GenericDrawable>()->setRenderGroup(2).setReceiveLights(true).castShadows();
    
    createChild("Hand")->
        createComponent<jop::GenericDrawable>(getRenderer())
        .setModel(jop::Model(jop::Mesh::getDefault()));
        findChild("Hand")->setPosition(findChild("Player")->getGlobalFront().x, findChild("Player")->getGlobalFront().y, findChild("Player")->getGlobalFront().z).setScale(0.2f, 0.2f, 0.2f);
        findChild("Hand")->getComponent<jop::GenericDrawable>()->setRenderGroup(3).setReceiveLights(false);

	jop::Material& gunMat = jop::ResourceManager::getEmptyResource<jop::Material>("mat");
	gunMat.setMap(jop::Material::Map::Diffuse, jop::ResourceManager::getResource<jop::Texture2D>("models/NailGun/nailgun.png", true));
	gunMat.setShininess(64.f);
	gunMat.setReflection(jop::Material::Reflection::Emission, jop::Color::Black);

	gun = createChild("gun");
	gun->createComponent<jop::ModelLoader>().load("models/NailGun/Nailgun.obj");
	gun->getComponent<jop::GenericDrawable>()->getModel().setMaterial(gunMat);
	gun->scale(0.25f).setPosition(-2.5f, -3.f, -4.f);
	gun->getComponent<jop::GenericDrawable>()->setRenderGroup(0).setReceiveLights(true);

	jop::Material& shoMat = jop::ResourceManager::getEmptyResource<jop::Material>("shomat");
	shoMat.setMap(jop::Material::Map::Diffuse, jop::ResourceManager::getResource<jop::Texture2D>("models/Shovel/Shovel.png", true));
	shoMat.setShininess(64.f);
	shoMat.setReflection(jop::Material::Reflection::Emission, jop::Color::Black);
	
	shovel = createChild("shovel");
	shovel->createComponent<jop::ModelLoader>().load("models/Shovel/Shovel.obj");
	shovel->getComponent<jop::GenericDrawable>()->getModel().setMaterial(shoMat);
	shovel->scale(0.25f).setPosition(-2.5f, -3.f, -4.f);
	shovel->getComponent<jop::GenericDrawable>()->setRenderGroup(3).setReceiveLights(false);
	
	jop::Material& hamMat = jop::ResourceManager::getEmptyResource<jop::Material>("hammat");
	hamMat.setMap(jop::Material::Map::Diffuse, jop::ResourceManager::getResource<jop::Texture2D>("models/Hammer/Hammer.png", true));
	hamMat.setShininess(64.f);
	hamMat.setReflection(jop::Material::Reflection::Emission, jop::Color::Black);

	hammer = createChild("hammer");
	hammer->createComponent<jop::ModelLoader>().load("models/Hammer/Hammer.obj");
	hammer->getComponent<jop::GenericDrawable>()->getModel().setMaterial(hamMat);
	hammer->scale(0.1f).setPosition(-2.5f, -3.f, -4.f);
	hammer->getComponent<jop::GenericDrawable>()->setRenderGroup(3).setReceiveLights(false);


    ///Envitorement
	jop::Material& Meteorite = jop::ResourceManager::getEmptyResource<jop::Material>("met");
	Meteorite.setMap(jop::Material::Map::Diffuse, jop::ResourceManager::getResource<jop::Texture2D>("models/Meteorite/Meteorite.png", true));
	Meteorite.setShininess(20.f);
	Meteorite.setReflection(jop::Material::Reflection::Diffuse, jop::Color::Orange);

	meteorite = createChild("meteorite");
	meteorite->createComponent<jop::ModelLoader>().load("models/Meteorite/Meteorite.obj");
	meteorite->getComponent<jop::GenericDrawable>()->getModel().setMaterial(Meteorite);
	meteorite->scale(20.f);
	meteorite->getComponent<jop::GenericDrawable>()->setRenderGroup(0).setReceiveLights(true).setCastShadows(true);
    meteorite->setPosition(2000.f, 1000.f, -500.f).setRotation(-1.f, 0.f, 0.f);
	meteorite->createComponent<jop::LightSource>(getRenderer(), jop::LightSource::Type::Point).setAttenuation(1000).setCastShadows(true).setIntensity(500);
    meteorite->getComponent<jop::LightSource>()->setRenderMask(0x00000005);
	meteoriteSpeed = 2000.f / (float(timeleft)-10.f);
	
	timer = createChild("ApocalypseTimer");
	timer->createComponent<jop::Text>(getRenderer()).setString("").setColor(jop::Color::Red);	
	timer->setPosition(0.f,0.f,0.f).setScale(2.f, 2.f, 1.f);

    jop::Material& wood= jop::ResourceManager::getEmptyResource<jop::Material>("wood");
    wood.setMap(jop::Material::Map::Diffuse, jop::ResourceManager::getResource<jop::Texture2D>("Level/wood.jpg", true));
    wood.setShininess(64.f);
    wood.setReflection(jop::Material::Reflection::Emission, jop::Color::Black);

    auto obj = createChild("Plank");
    obj->createComponent<jop::GenericDrawable>(getRenderer())
    .setModel(jop::Model(jop::Mesh::getDefault(),wood));
    findChild("Plank")->setPosition(-15.f, -3.f, -16.f).setScale(0.5f, 0.05f, 3.f);
    findChild("Plank")->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::BoxShape>("boxcoll", glm::vec3(0.5f, 0.05f, 3.0f)), jop::RigidBody::Type::Dynamic, 1.f));
    props.push_back(obj);
    props.push_back(cloneChild("Plank", "Plank"));
    props.push_back(cloneChild("Plank", "Plank"));
    props.push_back(cloneChild("Plank", "Plank"));
    props.push_back(cloneChild("Plank", "Plank"));
    props.push_back(cloneChild("Plank", "Plank"));
    props.push_back(cloneChild("Plank", "Plank"));
    props.push_back(cloneChild("Plank", "Plank"));
    props.push_back(cloneChild("Plank", "Plank"));
    props.push_back(cloneChild("Plank", "Plank"));
    props.push_back(cloneChild("Plank", "Plank"));
    props.push_back(cloneChild("Plank", "Plank2"));
    findChild("Plank2")->setPosition(-13.f, -5.f, -16.f);
    props.push_back(cloneChild("Plank2", "Plank2"));
    props.push_back(cloneChild("Plank2", "Plank2"));
    props.push_back(cloneChild("Plank2", "Plank2"));
    props.push_back(cloneChild("Plank2", "Plank2"));
    props.push_back(cloneChild("Plank2", "Plank2"));
    props.push_back(cloneChild("Plank2", "Plank2"));
    props.push_back(cloneChild("Plank2", "Plank2"));
    props.push_back(cloneChild("Plank2", "Plank2"));
    props.push_back(cloneChild("Plank2", "Plank2"));
    props.push_back(cloneChild("Plank2", "Plank2"));

	jop::Material& brick = jop::ResourceManager::getEmptyResource<jop::Material>("concrete");
	brick.setMap(jop::Material::Map::Diffuse, jop::ResourceManager::getResource<jop::Texture2D>("models/Brick/Brick.png", true));
	brick.setShininess(64.f);
	brick.setReflection(jop::Material::Reflection::Emission, jop::Color::Black);

	obj = createChild("brick");
	obj->createComponent<jop::ModelLoader>().load("models/Brick/Brick.obj");
	obj->getComponent<jop::GenericDrawable>()->getModel().setMaterial(brick);
	obj->scale(0.1f).setPosition(-11.f, -3.f, -16.f);
	obj->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::BoxShape>("brickcoll", glm::vec3(0.4f, 0.1f, 0.6f)), jop::RigidBody::Type::Dynamic, 1.f));
	props.push_back(obj);

	obj = createChild("brick");
	obj->createComponent<jop::ModelLoader>().load("models/Brick/Brick.obj");
	obj->getComponent<jop::GenericDrawable>()->getModel().setMaterial(brick);
	obj->scale(0.1f).setPosition(-11.f, -3.f, -16.f);
	obj->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::BoxShape>("brickcoll", glm::vec3(0.4f, 0.1f, 0.6f)), jop::RigidBody::Type::Dynamic, 1.f));
	props.push_back(obj);

	obj = createChild("brick");
	obj->createComponent<jop::ModelLoader>().load("models/Brick/Brick.obj");
	obj->getComponent<jop::GenericDrawable>()->getModel().setMaterial(brick);
	obj->scale(0.1f).setPosition(-11.f, -3.f, -16.f);
	obj->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::BoxShape>("brickcoll", glm::vec3(0.4f, 0.1f, 0.6f)), jop::RigidBody::Type::Dynamic, 1.f));
	props.push_back(obj);

	obj = createChild("brick");
	obj->createComponent<jop::ModelLoader>().load("models/Brick/Brick.obj");
	obj->getComponent<jop::GenericDrawable>()->getModel().setMaterial(brick);
	obj->scale(0.1f).setPosition(-11.f, -3.f, -16.f);
	obj->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::BoxShape>("brickcoll", glm::vec3(0.4f, 0.1f, 0.6f)), jop::RigidBody::Type::Dynamic, 1.f));
	props.push_back(obj);

	obj = createChild("brick");
	obj->createComponent<jop::ModelLoader>().load("models/Brick/Brick.obj");
	obj->getComponent<jop::GenericDrawable>()->getModel().setMaterial(brick);
	obj->scale(0.1f).setPosition(-11.f, -3.f, -16.f);
	obj->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::BoxShape>("brickcoll", glm::vec3(0.4f, 0.1f, 0.6f)), jop::RigidBody::Type::Dynamic, 1.f));
	props.push_back(obj);

	obj = createChild("brick");
	obj->createComponent<jop::ModelLoader>().load("models/Brick/Brick.obj");
	obj->getComponent<jop::GenericDrawable>()->getModel().setMaterial(brick);
	obj->scale(0.1f).setPosition(-11.f, -3.f, -16.f);
	obj->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::BoxShape>("brickcoll", glm::vec3(0.4f, 0.1f, 0.6f)), jop::RigidBody::Type::Dynamic, 1.f));
	props.push_back(obj);



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
	P1body.setAngularVelocity(glm::vec3(0.f, 0.f, 0.f));
	P1body.setAngularFactor(glm::vec3(0.f, 0.f, 0.f));
	
	second += dt;
	if (second >= 1.f)
	{
		second = 0.f;
		timeleft -= 1;
		if (timeleft == 10)
		{
				findChild("Skbo")->clearComponents();
			    findChild("Skbo")->createComponent<jop::SkyBox>(getRenderer()).setMap(jop::ResourceManager::getResource<jop::Cubemap>(
				"Level/skybox_rightDark.jpg", "Level/skybox_leftDark.jpg",
				"Level/skybox_topDark.jpg", "Level/skybox_bottomDark.jpg",
				"Level/skybox_backDark.jpg", "Level/skybox_frontDark.jpg", true));
				findChild("Sun")->getComponent<jop::LightSource>()->setAttenuation(100).setCastShadows(true).setIntensity(10);

		}

	}
	std::string Atime = "ApocalypseTimer: ";
	Atime += std::to_string(timeleft);
	Atime += 's';

	//Position updates
	meteorite->move(-meteoriteSpeed, 0.f, 0.f);

	if (holdRotation)
	{
		Camera->setRotation(cRot);
		holdRotation = false;
	}

	if (!objectPicked)
		handPos = 2.5f;

	if (P1->getGlobalPosition().x > 19.f)
	{
		P1body.setLinearVelocity(glm::vec3(0.f, 0.f, 0.f));
		P1body.applyImpulse(glm::vec3(-2.f, 0.5f, 0.f), P1->getGlobalPosition());
	}
	else if (P1->getGlobalPosition().x < -19.f)
	{
		P1body.setLinearVelocity(glm::vec3(0.f, 0.f, 0.f));
		P1body.applyImpulse(glm::vec3(2.f, 0.5f, 0.f), P1->getGlobalPosition());
	}
	else if (P1->getGlobalPosition().z > 19.f)
	{
		P1body.setLinearVelocity(glm::vec3(0.f, 0.f, 0.f));
		P1body.applyImpulse(glm::vec3(0.f, 0.5f, -2.f), P1->getGlobalPosition());
	}
	else if (P1->getGlobalPosition().z < -19.f)
	{
		P1body.setLinearVelocity(glm::vec3(0.f, 0.f, 0.f));
		P1body.applyImpulse(glm::vec3(0.f, 0.5f, 2.f), P1->getGlobalPosition());
	}
    Camera->setPosition(P1->getGlobalPosition().x, P1->getGlobalPosition().y + 2.f, P1->getGlobalPosition().z);
	Hand->setRotation(Camera->getLocalRotation());
	Hand->setPosition(Camera->getGlobalPosition());
	Hand->move(handPos * Camera->getGlobalFront());
   
	gun->setRotation(Camera->getGlobalRotation());
    gun->rotate(glm::vec3(0.f, 1.65f, 0.2f));
    gun->setPosition(Camera->getGlobalPosition());
    gun->move(Camera->getGlobalFront() + 0.5f*Camera->getGlobalRight() + 0.1f*Camera->getGlobalUp());

	shovel->setRotation(Camera->getGlobalRotation());
	shovel->rotate(glm::vec3(-0.5f, 0.f, 0.2f));
	shovel->setPosition(Camera->getGlobalPosition());
	
	if (currentTool==2)
	shovel->move(Camera->getGlobalFront() + 0.5f*Camera->getGlobalRight() + -1.2f*Camera->getGlobalUp());
	else
		shovel->move(-1.f*Camera->getGlobalFront() + 0.5f*Camera->getGlobalRight() + -1.2f*Camera->getGlobalUp());

	hammer->setRotation(Camera->getGlobalRotation());
	hammer->rotate(glm::vec3(0.f, 0.f, 0.2f));
	hammer->setPosition(Camera->getGlobalPosition());

	if (currentTool == 1)
		hammer->move(Camera->getGlobalFront() + 0.5f*Camera->getGlobalRight() + -0.5f*Camera->getGlobalUp());
	else
		hammer->move(-1.f*Camera->getGlobalFront() + 0.5f*Camera->getGlobalRight() + -0.5f*Camera->getGlobalUp());


    lastrot = Camera->getGlobalFront();
    lastpos = Camera->getGlobalPosition();
    ///INPUT
    auto& h = *jop::Engine::getSubsystem<jop::Window>()->getEventHandler();
    using jop::Keyboard;
	if (h.keyDown(Keyboard::One))
	{
		currentTool = 0;
		gun->getComponent<jop::GenericDrawable>()->setRenderGroup(0).setReceiveLights(true);
		shovel->getComponent<jop::GenericDrawable>()->setRenderGroup(3).setReceiveLights(false);
		hammer->getComponent<jop::GenericDrawable>()->setRenderGroup(3).setReceiveLights(false);
	}
	if (h.keyDown(Keyboard::Two))
	{
		currentTool = 1;
		gun->getComponent<jop::GenericDrawable>()->setRenderGroup(3).setReceiveLights(false);
		shovel->getComponent<jop::GenericDrawable>()->setRenderGroup(3).setReceiveLights(false);
		hammer->getComponent<jop::GenericDrawable>()->setRenderGroup(0).setReceiveLights(true);
	}
	if (h.keyDown(Keyboard::Three))
	{
		currentTool = 2;
		gun->getComponent<jop::GenericDrawable>()->setRenderGroup(3).setReceiveLights(false);
		shovel->getComponent<jop::GenericDrawable>()->setRenderGroup(0).setReceiveLights(true);
		hammer->getComponent<jop::GenericDrawable>()->setRenderGroup(3).setReceiveLights(false);
	}

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
			if (P1body.checkContact(*findChild("groundBox")->getComponent<jop::RigidBody>()))
            P1body.setLinearVelocity(5.f*P1->getGlobalUp());
			else
			{
				for (auto it = props.begin(); it != props.end(); ++it)
				{
					if (P1body.checkContact(*it->get()->getComponent<jop::RigidBody>()))
					{
						P1body.setLinearVelocity(5.f*P1->getGlobalUp());
						break;
					}
				}
			}
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
            if (!objectPicked)
            {
                for (prop = props.begin(); prop != props.end(); ++prop)
                {
                    glm::vec3 difference = (Camera->getGlobalPosition() - prop->get()->getGlobalPosition());
                    float distance = sqrt(difference.x*difference.x + difference.y*difference.y + difference.z*difference.z);

                    glm::vec3 up = { 0.f, 1.f, 0.f };
                    glm::mat4 invPanthom = Camera->getInverseTransform().getMatrix();
                    invPanthom = glm::lookAt(Camera->getGlobalPosition(), prop->get()->getGlobalPosition(), up);
                    glm::mat4 panthom = glm::inverse(invPanthom);
                    glm::vec3 front = glm::normalize(-glm::vec3(panthom[2][0], panthom[2][1], panthom[2][2]));
                    float direction = sqrt(Camera->getGlobalFront().x*front.x + Camera->getGlobalFront().y*front.y + Camera->getGlobalFront().z*front.z);

                    if (distance <= 7.f&&direction > 0.99f)
                        objectPicked = true;

                    if (objectPicked)
                        break;
                }
            }

        }
        if (!h.keyDown(Keyboard::E))
        {
            objectPicked = false;
			if (currentTool == 1 && h.mouseButtonDown(jop::Mouse::Left))
			{
				for (auto it = props.begin(); it != props.end(); ++it)
				{
					glm::vec3 difference = (Camera->getGlobalPosition() - it->get()->getGlobalPosition());
					float distance = sqrt(difference.x*difference.x + difference.y*difference.y + difference.z*difference.z);

					glm::vec3 up = { 0.f, 1.f, 0.f };
					glm::mat4 invPanthom = Camera->getInverseTransform().getMatrix();
					invPanthom = glm::lookAt(Camera->getGlobalPosition(), it->get()->getGlobalPosition(), up);
					glm::mat4 panthom = glm::inverse(invPanthom);
					glm::vec3 front = glm::normalize(-glm::vec3(panthom[2][0], panthom[2][1], panthom[2][2]));
					float direction = sqrt(Camera->getGlobalFront().x*front.x + Camera->getGlobalFront().y*front.y + Camera->getGlobalFront().z*front.z);

					if (distance <= 7.f&&direction > 0.99f&&glm::pow(it->get()->getComponent<jop::RigidBody>()->getGravity().y, 2.f) < 1.f)
					{
						it->get()->getComponent<jop::RigidBody>()->removeSelf();
						if(it->get()->getLocalScale() == glm::vec3(0.1f, 0.1f, 0.1f))
							it->get()->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::BoxShape>("brickcoll", glm::vec3(0.4f, 0.1f, 0.6f)), jop::RigidBody::Type::Dynamic, 1.f));
						else
							it->get()->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::BoxShape>("boxcoll", glm::vec3(0.5f, 0.05f, 3.0f)), jop::RigidBody::Type::Dynamic, 1.f));

						break;
					}
				}
			}
			else if (currentTool == 2 && h.mouseButtonDown(jop::Mouse::Left))
			{
			
			}
        }

         if (objectPicked)
         {
                 glm::vec3 difference = (Camera->getGlobalPosition() - prop->get()->getGlobalPosition());
            float hor = 0.f;
            float ver = 0.f;

            glm::vec3 force = { hor, 5.f*(Camera->getGlobalPosition().y - prop->get()->getGlobalPosition().y), ver };

            float distance = sqrt((prop->get()->getGlobalPosition().x - Hand->getGlobalPosition().x)*(prop->get()->getGlobalPosition().x - Hand->getGlobalPosition().x)
                + (prop->get()->getGlobalPosition().y - Hand->getGlobalPosition().y)*(prop->get()->getGlobalPosition().y - Hand->getGlobalPosition().y)
                + (prop->get()->getGlobalPosition().z - Hand->getGlobalPosition().z)*(prop->get()->getGlobalPosition().z - Hand->getGlobalPosition().z));
			
            if (prop->get()->getGlobalPosition().x != Hand->getGlobalPosition().x)
            {
				force.x = (prop->get()->getGlobalPosition().x < Hand->getGlobalPosition().x ? 1.f : -1.f)
					*distance*2.f;
            }
            if (prop->get()->getGlobalPosition().y != Camera->getGlobalPosition().y)
            {
                force.y = (prop->get()->getGlobalPosition().y < Hand->getGlobalPosition().y ? 1.f : -1.f)
					*distance*2.f;
            }
            if (prop->get()->getGlobalPosition().z != Camera->getGlobalPosition().z)
            {
                force.z = (prop->get()->getGlobalPosition().z < Hand->getGlobalPosition().z ? 1.f : -1.f)
					*distance*2.f;
            }
                prop->get()->getComponent<jop::RigidBody>()->setLinearVelocity(force);

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

				if (h.keyDown(Keyboard::KeypadAdd))
				{
					if (handPos < 5.f)
						handPos+=0.5f;
				}

				if (h.keyDown(Keyboard::KeypadSubtract))
				{
					if (handPos > 1.f)
						handPos -= 0.5f;
				}

				if (h.mouseButtonDown(jop::Mouse::Right))
				{
					holdRotation = true;
					cRot = Camera->getLocalRotation();
					cPos-=h.getCursorPosition();
					force.y = -cPos.x;
					force.z = -cPos.y;
				}

				prop->get()->getComponent<jop::RigidBody>()->setAngularVelocity(force);

				if (currentTool==0)
				{ 
					if (h.mouseButtonDown(jop::Mouse::Left))
					{
						if (prop->get()->getLocalScale() == glm::vec3(0.1f, 0.1f, 0.1f) && prop->get()->getComponent<jop::RigidBody>()->checkContact(*findChild("groundBox")->getComponent<jop::RigidBody>()))
						{
							prop->get()->getComponent<jop::RigidBody>()->removeSelf();
							prop->get()->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::BoxShape>("StaticBrick", glm::vec3(0.4f, 0.1f, 0.6f)), jop::RigidBody::Type::Static, 1.f));
							prop->get()->getComponent<jop::RigidBody>()->setGravity(glm::vec3(0.f, 0.f, 0.f));
						}
						else if (prop->get()->getLocalScale() != glm::vec3(0.1f, 0.1f, 0.1f))
						{
							for (auto it = props.begin(); it != props.end(); ++it)
							{
								if (prop == it)
									++it;

								if (prop->get()->getComponent<jop::RigidBody>()->checkContact(*it->get()->getComponent<jop::RigidBody>()) && glm::pow(it->get()->getComponent<jop::RigidBody>()->getGravity().y, 2.f) < 1.f)
								{
									prop->get()->getComponent<jop::RigidBody>()->removeSelf();
									prop->get()->createComponent<jop::RigidBody>(getWorld(), jop::RigidBody::ConstructInfo(jop::ResourceManager::getNamedResource<jop::BoxShape>("StaticWood", glm::vec3(0.5f, 0.05f, 3.0f)), jop::RigidBody::Type::Static, 1.f));
									prop->get()->getComponent<jop::RigidBody>()->setGravity(glm::vec3(0.f, 0.f, 0.f));
								}
							}
						}
					}
                }
          }
        
		  timer->clearComponents();
		  timer->createComponent<jop::Text>(getRenderer()).setString(Atime).setColor(jop::Color::Red);
		  timer->setRotation(Camera->getGlobalRotation());
		  timer->setPosition(Camera->getGlobalPosition());
		  timer->move(2.f*Camera->getGlobalFront() + 0.2f*Camera->getGlobalRight() + 0.9f*Camera->getGlobalUp());

		  cPos = h.getCursorPosition();
}