// rotater.hpp

#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

#include "Component.hpp"
#include "Transform.hpp"


namespace sl {

    class Rotater : public Component {
    public:
        void Update(const float deltaTime) override
        {
            ImGui::Begin("Spin");
            ImGui::SliderFloat("Scale", &Transform()->Rotation, .0f, 360.0f, "%fpx");
            ImGui::End();
        }

    private:

    };

}