/*
** EPITECH PROJECT, 2023
** R-type
** File description:
** animeSystem
*/

#ifndef ANIMESYSTEM_HPP_
#define ANIMESYSTEM_HPP_

#include "../../ecs/system/ISystem.hpp"
#include "../../ecs/registry/registry.hpp"


class AnimeSystem : public ISystem {
    public:
        AnimeSystem() = delete;
        AnimeSystem(registry &reg): _reg(reg) {};
        ~AnimeSystem() = default;
        void operator()() override {
            auto &sprite = _reg.getComponent<Sprite>();
            auto &anime = _reg.getComponent<Anime>();

            for (long unsigned int i = 0; i < anime.size(); i++) {
                if (!sprite[i] || !anime[i])
                    continue;
                auto &sprite_value = sprite[i].value();
                auto &anime_value = anime[i].value();
                if (anime_value.internClock == anime_value.targetClock) {
                    sprite_value.rectLeft += anime_value.incrementLeft;
                    if (sprite_value.rectLeft >= anime_value.maxWidth)
                        sprite_value.rectLeft = 0;
                    anime_value.internClock = 0;
                } else {
                    anime_value.internClock++;
                }
            }
        };
    private:
        registry &_reg;
};

#endif /* !ANIMSYSTEM_HPP_ */
