// rescaler.hpp

#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

#include "vector.hpp"
#include "Component.hpp"
#include "transform.hpp"


namespace sl {

    class RescalerUniform : public Component {
    public:
        void Update(const float deltaTime) override
        {
            float size = Transform()->Scale.x;

            ImGui::Begin("Scale me baby");
            ImGui::InputFloat((Owner->name + ": Lower Bound").c_str(), &lowerBound);
            ImGui::InputFloat((Owner->name + ": Upper Bound").c_str(), &upperBound);
            ImGui::SliderFloat((Owner->name + ": X").c_str(), &size, lowerBound, upperBound, "%fpx");
            ImGui::End();

            Transform()->Scale = Vector2::One * size;
        }

    private:
        float lowerBound = .0f;
        float upperBound = 2.0f;
    };

    class Rescaler : public Component {
    public:
        void Update(const float deltaTime) override
        {
            ImGui::Begin("Scale me baby");
            ImGui::InputFloat((Owner->name + ": Lower Bound").c_str(), &lowerBound);
            ImGui::InputFloat((Owner->name + ": Upper Bound").c_str(), &upperBound);
            ImGui::SliderFloat((Owner->name + ": X").c_str(), &Transform()->Scale.x, lowerBound, upperBound, "%fpx");
            ImGui::SliderFloat((Owner->name + ": Y").c_str(), &Transform()->Scale.y, lowerBound, upperBound, "%fpx");
            ImGui::End();
        }

    private:
        float lowerBound = .0f;
        float upperBound = 2.0f;
    };

}