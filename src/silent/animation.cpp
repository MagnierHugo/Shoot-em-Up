// animation.cpp

#include "animation.hpp"
#include "Renderable.hpp"
#include "renderer.hpp"


namespace sl {
    Animation Animation::LoadFromSpriteSheet(const sf::Texture& sprite_sheet, const sf::Vector2i& sprites_size, const u8 _frames_between_sprites) {
        Animation animation;
        animation.frames_between_sprites = _frames_between_sprites;

        u32 rows = sprite_sheet.getSize().y / sprites_size.y;
        u32 columns = sprite_sheet.getSize().x / sprites_size.x;
        sf::IntRect sprite_rect = sf::IntRect(sf::Vector2i(0, 0), sprites_size);
        
        for (int i = 0; i < rows; i++) {
            sprite_rect.top += sprites_size.y;
            for (int j = 0; j < columns; j++) {
                sprite_rect.left = sprites_size.x * j;
                animation.sprites.push_back(sf::Sprite(sprite_sheet, sprite_rect));
            }
        }
        return animation;
    }

    sf::Sprite Animation::Play() {
        if (++frames_passed == frames_between_sprites) { 
            current_sprite = current_sprite == sprites.size() - 1 ? 0 : current_sprite + 1;
            frames_passed = 0;
        }
        return sprites[current_sprite];
    }


    void Animator::Awake() {
        renderable = Owner->GetComponent<Renderable>();
    }

    void Animator::Update(const float deltaTime) {
        // renderable->SetModeSprite();
    }

    void Animator::AddAnimation(const std::string& animation_name, const sf::Texture& sprite_sheet, const sf::Vector2i& sprites_size, const u8 _frames_between_sprites) {
        if (animations.find(animation_name) != animations.end()) {
            animations[animation_name] = Animation::LoadFromSpriteSheet(sprite_sheet, sprites_size, _frames_between_sprites);
        }
    }

    void Animator::RemoveAnimation(const std::string& animation_name) {
        if (animations.find(animation_name) != animations.end()) {
            animations.erase(animation_name);
        }
    }

    void Animator::AddTransitions(const std::string& animation_name, const std::vector<std::string>& transition_to) {
        if (transition_table.find(animation_name) != transition_table.end()) {
            transition_table[animation_name] = transition_to;
        }
    }

    void Animator::SetAnimation(std::string& animation_name) {
        if (current_animation.empty()){
            current_animation = animation_name;
            return;
        }

        if ((current_animation == animation_name) || (transition_table.find(current_animation) == transition_table.end())) { return; }

        if (Contains(transition_table[current_animation], animation_name)) {
            current_animation = animation_name;
        }
    }
};