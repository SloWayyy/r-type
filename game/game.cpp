/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** game
*/

#include "game.hpp"

void Game::init()
{
    reg.addAllComponents<Position, Sprite, Size>();
    gameEntity.setShip(reg.addEntity());
    size_t ship = gameEntity.getShip();

    auto &position = reg.getComponent<Position>();
    auto &sprite = reg.getComponent<Sprite>();
    auto &size = reg.getComponent<Size>();

    position.emplace_at(ship, 500, 100);
    sprite.emplace_at(ship, "game/assets/ship.png");
    size.emplace_at(ship, 0.5, 0.1);
}

void Game::run()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    sf::Event event;
    init();
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        sys.drawEntity(window);
        window.display();
    }
}
