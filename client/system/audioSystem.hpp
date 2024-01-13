/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** audioSystem
*/

#ifndef AUDIOSYSTEM_HPP_
    #define AUDIOSYSTEM_HPP_

    #include "../../ecs/system/ISystem.hpp"
    #include "../../network/tcpClient/tcpClient.hpp"
    #include "../../network/udp/udp.hpp"
    #include "../../ecs/event/keyPressed.hpp"
    #include "../../ecs/event/shoot.hpp"

class AudioSystem : public ISystem {
    public:
        AudioSystem() = delete;
        AudioSystem(registry &reg): _reg(reg) {
            if (!bulletBuffer.loadFromFile("../client/bullet.ogg")) {
                std::cerr << "Failed to load bullet sound file." << std::endl;
                return;
            }
            bulletSound.setBuffer(bulletBuffer);
        };
        ~AudioSystem() = default;
        void operator()() override {
            if(_reg._eventManager.checkEvent<shoot>()) {
                auto &tmp = _reg._eventManager.getEvent<shoot>();
                for_each(tmp.begin(), tmp.end(), [this](auto &) {
                    playBulletSound();
                });
            }
        };
    private:
        registry &_reg;
        sf::SoundBuffer bulletBuffer;
        sf::Sound bulletSound;
        void playBulletSound() {
            bulletSound.play();
        }
};

#endif /* !AUDIOSYSTEM_HPP_ */
