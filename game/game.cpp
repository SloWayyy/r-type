/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** game
*/

#include "game.hpp"

void Game::init()
{
    reg.addAllComponents<Position, Sprite, Size, Velocity>();
    gameEntity.setShip(reg.addEntity());
    size_t ship = gameEntity.getShip();

    auto &position = reg.getComponent<Position>();
    auto &sprite = reg.getComponent<Sprite>();
    auto &size = reg.getComponent<Size>();
    auto &velocity = reg.getComponent<Velocity>();

    position.emplace_at(ship, 50, 50);
    sprite.emplace_at(ship, "game/assets/spaceShip.png", sf::IntRect(198, 0, 32, 32));
    velocity.emplace_at(ship, 0, 0, 0, 10, 0);
    size.emplace_at(ship, 1.5, 1.5);
    reg.add_system(moveEntity);
}

void Game::run()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    window.setFramerateLimit(5);
    sf::Event event;

    init();
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();

        animeEntity(32, 198, reg);
        drawEntity(window, reg);
        reg.run_system();
        window.display();
    }
}
