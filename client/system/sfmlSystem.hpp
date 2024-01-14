/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** system
*/

#ifndef SFMLSYSTEM_HPP_
    #define SFMLSYSTEM_HPP_

    #include "../../ecs/system/ISystem.hpp"
    #include "../../ecs/registry/registry.hpp"
    #include <SFML/Graphics.hpp>
    #include <filesystem>
    #include <vector>

class SfmlSystem : public ISystem {
    public:
        SfmlSystem() = delete;
        SfmlSystem(registry &reg, std::string assetsPath, unsigned int width = 800, unsigned int height = 600, std::string windowName = "R-Type"):
        _reg(reg), _window(sf::VideoMode(width, height), windowName), _assetsPath(assetsPath)
        {
            try {
                if (!std::filesystem::exists(assetsPath))
                    throw std::runtime_error("Assets path does not exist");
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
                exit(84);
            }
            for (auto &file : std::filesystem::directory_iterator{assetsPath}) {
                _assetName.push_back(file.path().string());
            }
            std::sort(_assetName.begin(), _assetName.end());
            for_each(_assetName.begin(), _assetName.end(), [this](auto &file) {
                sf::Texture texture;
                if (texture.loadFromFile(file)) {
                    this->_textures.push_back(std::move(texture));
                    std::cout << file << " Loaded" << std::endl;
                }
            });
            try {
                if (!std::filesystem::exists(assetsPath + "/font"))
                    throw std::runtime_error("Assets path does not exist");
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
                exit(84);
            }
            for (auto &file : std::filesystem::directory_iterator{assetsPath + "/font"}) {
                sf::Font font;
                if (font.loadFromFile(file.path().string())) {
                    this->_fonts.push_back(std::move(font));
                    std::cout << file.path().string() << " Loaded" << std::endl;
                }
            }
        };
        ~SfmlSystem() = default;
        void operator()() override {
            eventHandler();
            drawSprite();
        };
    private:
        void drawSprite() {
            _window.clear(sf::Color::Cyan);
            auto &sprite_array = _reg.getComponent<Sprite>();
            auto &position = _reg.getComponent<Position>();
            auto &size = _reg.getComponent<Size>();
            auto &score = _reg.getComponent<Score>();
            sf::Sprite spriteBg;
            spriteBg.setTexture(_textures[6]);
            _window.draw(spriteBg);
            auto &health = _reg.getComponent<Health>();

            sf::Sprite spriteGameOver;
            spriteGameOver.setTexture(_textures[7]);
            spriteGameOver.setScale(1.25, 1.7);
            sf::Sprite spriteGameWin;
            spriteGameWin.setTexture(_textures[8]);
            spriteGameWin.setScale(2, 1.5);

            for (long unsigned int i = 0; i < sprite_array.size(); i++) {
                if (!sprite_array[i] || !position[i] || !size[i])
                    continue;
                if (i <= 3 && health[i] && health[i].value().health <= 0)
                    continue;
                auto &sprite_val = sprite_array[i].value();
                auto &pos_value = position[i].value();
                auto &size_value = size[i].value();

                sf::Sprite sprite;
                sprite.setTexture(_textures[sprite_val.textureId]);
                sprite.setTextureRect(sf::IntRect(sprite_val.rectLeft, sprite_val.rectTop, sprite_val.rectWidth, sprite_val.rectHeight));
                sprite.setPosition(pos_value.x, pos_value.y);
                sprite.setScale(size_value.w, size_value.h);
                _window.draw(sprite);
            }
            int cpt = 0;
            for (uint32_t i = 0; i < 4; i++) {
                if (health[i] && i != _reg._player && health[i].value().health <= 0) {
                    cpt++;
                }
            }
            if (health[_reg._player]) {
                if (_reg._player <= 3) {
                    sf::Text text("Life: " + std::to_string(health[_reg._player].value().health), _fonts[0]);
                    text.setCharacterSize(30);
                    text.setFillColor(sf::Color::Red);
                    text.setPosition(10, 0);
                    _window.draw(text);
                }
            }
            if (score[_reg._player]) {
                if (_reg._player <= 3) {
                    sf::Text textScore("Score: " + std::to_string(score[_reg._player].value().s), _fonts[0]);
                    textScore.setCharacterSize(30);
                    textScore.setFillColor(sf::Color::Blue);
                    textScore.setPosition(650, 0);
                    _window.draw(textScore);
                }
            }
            if (score[_reg._player]) {
                if (_reg._player > 3) {
                    sf::Text textSpec("Mode spectator", _fonts[0]);
                    textSpec.setCharacterSize(30);
                    textSpec.setFillColor(sf::Color::Yellow);
                    textSpec.setPosition(250, 0);
                     _window.draw(textSpec);
                }
            }
            if (score[_reg._player]) {
                if (_reg._player <= 3) {
                    sf::Text textSpec("Player: " + std::to_string(_reg._player), _fonts[0]);
                    textSpec.setCharacterSize(20);
                    textSpec.setFillColor(sf::Color::White);
                    textSpec.setPosition(680, 570);
                     _window.draw(textSpec);
                }
            }
            if (cpt == 3 && health[_reg._player] && health[_reg._player].value().health > 0) {
                _window.draw(spriteGameWin);
            }
            if (health[_reg.getPlayerEntity()] && health[_reg.getPlayerEntity()].value().health <= 0 && cpt != 3)
                _window.draw(spriteGameOver);
            _window.display();
        }

        void eventHandler() {
            while (_window.pollEvent(_event)) {
                switch(_event.type) {
                    case sf::Event::Closed:
                        _window.close();
                        break;
                    case sf::Event::TextEntered:
                        if (_event.text.unicode < 'z' && _event.text.unicode != 8) {
                            _reg._eventManager.addEvent<textEntered>(_event.text.unicode);
                        }
                        break;
                    case sf::Event::KeyPressed:
                        _reg._eventManager.addEvent<keyPressed>(_event.key.code);
                        break;
                    default:
                        break;
                }
            }
        }
        sf::Event _event;
        std::vector<std::string> _assetName;
        std::vector<sf::Texture> _textures;
        std::vector<sf::Font> _fonts;
        registry &_reg;
        sf::RenderWindow _window;
        std::string _assetsPath;
};


#endif /* !SFMLSYSTEM_HPP_ */
