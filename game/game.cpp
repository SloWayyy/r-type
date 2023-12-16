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

    Position pos = {10, 100};

    char *test = reinterpret_cast<char*>(&pos);
    Position pos2 = reinterpret_cast<Position&>(*test);

    Sprite spr = {"game/assets/spaceShip.png", sf::IntRect(198, 0, 32, 32)};

    char *test2 = reinterpret_cast<char*>(&spr);
    sprite.insert_packet(0, test2);
    // Sprite spr2 = reinterpret_cast<Sprite&>(*test2);


    pos2.print();
    position.emplace_at(ship, 50, 50);
    position.insert_packet(ship, test);

    // sprite.emplace_at(ship, "game/assets/spaceShip.png", sf::IntRect(198, 0, 32, 32));
    velocity.emplace_at(ship, 0, 0, 0, 10, 0);
    size.emplace_at(ship, 1.5, 1.5);
    // reg.add_system(moveEntity);
    reg.add_system(animeEntity, 32, 198);
}

void Game::run()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    reg.add_system(drawEntity, std::ref(window));
    window.setFramerateLimit(5);
    sf::Event event;

    init();
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        reg.run_system();
        window.display();
    }
}
