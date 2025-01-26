#pragma once

#include <iostream>

#include "scene.hpp"
#include "scenes_manager.hpp"
#include "component.hpp"
#include "entity.hpp"
#include "input_manager.hpp"


namespace sl {

    class LoadSceneOnInput : public Component {
    public:
        void Update(const float) override;
        void Init(std::string scene_, sf::Keyboard::Key input_);
        
    private:
        std::string scene;
        sf::Keyboard::Key input = sf::Keyboard::Key::Unknown;

    };

}