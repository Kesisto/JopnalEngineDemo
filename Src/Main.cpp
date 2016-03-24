#include <Jopnal/Jopnal.hpp>

int main(int c, char* v[])
{
    JOP_ENGINE_INIT("EngineDemo", c, v);

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
                jop::StateLoader::getInstance().loadState("Scene/test", true, true, true);
            else if (key == jop::Keyboard::K)
                jop::StateLoader::getInstance().saveState("Scene/test", true, true, true);

            if (key == jop::Keyboard::Escape)
                closed();
        }

        void mouseMoved(const float x, const float y) override
        {
            auto& cam = *jop::Camera::getDefault().getObject();

            static float mx = 0.f;
            static float my = 0.f;
            mx += x;
            my = glm::clamp(my + y, -85.f, 85.f);

            cam.setRotation(glm::radians(-my), glm::radians(-mx), 0.f);
        }
    };

    jop::Engine::getSubsystem<jop::Window>()->setMouseMode(jop::Mouse::Mode::Frozen);
    jop::Engine::getSubsystem<jop::Window>()->setEventHandler<EventHandler>();

    if (&jop::ShaderManager::getShader(jop::Material::DefaultAttributes) == &jop::Shader::getDefault())
        return EXIT_FAILURE;

    return JOP_MAIN_LOOP;
}