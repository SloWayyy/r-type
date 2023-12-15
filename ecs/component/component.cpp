#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

struct Position {
    Position(int x = 0, int y = 0)
        : x(x), y(y){};
    void print()
    {
        std::cout << "Position x: " << x << "\nPosition y: " << y << std::endl;
    };
    int x;
    int y;
};

struct Size {
    Size(float width = 0, float height = 0)
        : w(width), h(height){};
    float w;
    float h;
};

struct Velocity {
    Velocity(int velocity = 0, int time = 0, int acc = 0, int x_speed = 0, int y_speed = 0): v(velocity), t(time), a(acc), x_speed(x_speed), y_speed(y_speed){};
    void print()
    {
        std::cout << "Velocity v: " << v << "\nVelocity t: " << t << "\nVelocity a: " << a << std::endl;
    };
    int x_speed;
    int y_speed;
    int v;
    int t;
    int a;
};

struct Drawable {
    Drawable(bool drawable = true)
        : isDrawable(drawable){};
    bool isDrawable;
};

struct Controller {
    Controller(bool controller = true)
        : isController(controller){};
    bool isController;
};

struct Sprite {
    Sprite(std::string path, sf::IntRect rect) : rect(rect) {
        if (!texture.loadFromFile(path)) {
            std::cerr << "Error loading texture" << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setTextureRect(rect);
        std::cout << "Sprite created with path: " << path << std::endl;
    }
    sf::IntRect rect;
    sf::Texture texture;
    sf::Sprite sprite;

    Sprite(const Sprite &other)
    {
        rect = other.rect;
        texture = other.texture;
        sprite = other.sprite;
        sprite.setTexture(texture);
    }

};
