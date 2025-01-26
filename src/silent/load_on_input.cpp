#include "load_on_input.hpp"


namespace sl {

    void LoadSceneOnInput::Update(const float)
    {
        if (sl::InputManager::GetKeyDown(input))
        {
            sl::ScenesManager::LoadScene(scene);
        }
    }

    void LoadSceneOnInput::Init(std::string scene_, sf::Keyboard::Key input_) {
        scene = scene_;
        input = input_;
    }

}
