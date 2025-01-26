// animation.hpp

#include <vector>
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>

#include "common.hpp"
#include "Component.hpp"

namespace sl {

    class Renderable;

    struct Animation {
        std::vector<sf::Sprite> sprites;
        u8 current_sprite;

        u8 frames_between_sprites;
        u8 frames_passed;

        static Animation LoadFromSpriteSheet(const sf::Texture& sprite_sheet, const sf::Vector2i& sprites_size, const u8 _frames_between_sprites);
        sf::Sprite Play();
    };

    class Animator : public Component {
    public:
        void Awake() override;
        void Update(const float deltaTime) override;

        void AddAnimation(const std::string& animation_name, const sf::Texture& sprite_sheet, const sf::Vector2i& sprites_size, const u8 _frames_between_sprites);
        void RemoveAnimation(const std::string& animation_name);
        void AddTransitions(const std::string& animation_name, const std::vector<std::string>& transition_to);
        void SetAnimation(std::string& animation_name);

    private:
        Ref<Renderable> renderable;
        std::unordered_map<std::string, Animation> animations;
        std::unordered_map<std::string, std::vector<std::string>> transition_table;
        std::string current_animation;

    };

};