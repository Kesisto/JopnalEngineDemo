
#include <Jopnal/Jopnal.hpp>
#include <Scenes/Splash.hpp>

JOP_REGISTER_LOADABLE(a, Splash)[](std::unique_ptr<jop::Scene>& s, const jop::json::Value& val)
{
    auto sc = std::make_unique<Splash>();
    sc->m_sine = static_cast<float>(val["sine"].GetDouble());

    s = std::move(sc);
    return true;
}
JOP_END_LOADABLE_REGISTRATION(SomeScene)

JOP_REGISTER_SAVEABLE(a, Splash)[](const jop::Scene& s, jop::json::Value& v, jop::json::Value::AllocatorType& a) -> bool
{
    v.AddMember(jop::json::StringRef("id"), jop::json::StringRef(s.getID().c_str()), a)
        .AddMember(jop::json::StringRef("active"), s.isActive(), a)
        .AddMember(jop::json::StringRef("sine"), static_cast<const Splash&>(s).m_sine, a);

    return true;
}
JOP_END_SAVEABLE_REGISTRATION(SomeScene)


int main(int c, char* v[])
{
    JOP_ENGINE_INIT("EngineDemo", c , v);

    struct EventHandler : jop::WindowEventHandler
    {
        EventHandler(jop::Window& w)
            : jop::WindowEventHandler(w)
        {}

        void closed() override
        {
            jop::Engine::exit();
        }
        void keyPressed(const int key, const int, const int) override
        {
            if (key == jop::Keyboard::L)
                jop::StateLoader::getInstance().loadState("Scene/test", true, true);
            else if (key == jop::Keyboard::K)
                jop::StateLoader::getInstance().saveState("Scene/test", true, true);
            else if (key == jop::Keyboard::Comma)
                jop::Engine::getCurrentScene().getWorld().setDebugMode(!jop::Engine::getCurrentScene().getWorld().debugMode());
            else if (key == jop::Keyboard::Period)
            {
                auto& obj = *jop::Engine::getCurrentScene().findChild("envmap");
                obj.setActive(!obj.isActive());
            }
            else if (key == jop::Keyboard::P)
                jop::Engine::setState(jop::Engine::getState() == jop::Engine::State::Running ? jop::Engine::State::RenderOnly : jop::Engine::State::Running);
            else if (key == jop::Keyboard::F)
                jop::Engine::advanceFrame();

            if (key == jop::Keyboard::Escape)
                closed();
        }

        void mouseMoved(const float x, const float y) override
        {
            auto& cam = *jop::Engine::getCurrentScene().findChild("Cam");

            static float mx = 0.f;
            static float my = 0.f;
            mx += x;
            my = glm::clamp(my + y, -85.f, 85.f);

            cam.setRotation(glm::radians(-my), glm::radians(-mx), 0.f);
        }
    };

    jop::Engine::getSubsystem<jop::Window>()->setMouseMode(jop::Mouse::Mode::Frozen);
    jop::Engine::getSubsystem<jop::Window>()->setEventHandler<EventHandler>();

    if (&jop::ShaderManager::getShader(jop::Material::Attribute::Default) == &jop::Shader::getError())
        return EXIT_FAILURE;

    jop::Engine::createScene<Splash>();

    return JOP_MAIN_LOOP;
}