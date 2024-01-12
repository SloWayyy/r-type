#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <iostream>

struct Position {
    Position(int x = 0, int y = 0)
        : x(x)
        , y(y) {};
    void print() { std::cout << "Position x: " << x << " Position y: " << y << std::endl; };
    int x;
    int y;
};

struct Health {
    Health(int health = 0)
        : health(health) {};
    void print() { std::cout << "Health: " << health << std::endl; };
    int health;
};

struct HitBox {
    HitBox(int width = 0, int height = 0)
        : w(width)
        , h(height) {};
    void print() { std::cout << "HitBox w: " << w << " HitBox h: " << h << std::endl; };
    int w;
    int h;
};

struct Size {
    Size(float width = 0, float height = 0)
        : w(width)
        , h(height) {};
    void print() { std::cout << "Size w: " << w << " Size h: " << h << std::endl; };
    float w;
    float h;
};

struct Velocity {
    Velocity(int x_speed = 0, int y_speed = 0)
        : x_speed(x_speed)
        , y_speed(y_speed) {};
    void print() { std::cout << "Velocity x: " << x_speed << " Velocity y: " << y_speed << std::endl; };
    int x_speed;
    int y_speed;
};

struct Sprite {
    Sprite(uint8_t textureId, uint32_t left, uint32_t top, uint32_t width, uint32_t height)
        : textureId(textureId)
        , rectLeft(left)
        , rectTop(top)
        , rectWidth(width)
        , rectHeight(height)
    {
    }
    void print() { std::cout << "Sprite textureId: " << static_cast<uint32_t>(textureId) << " Sprite rectLeft: " << rectLeft << " Sprite rectTop: " << rectTop << " Sprite rectWidth: " << rectWidth << " Sprite rectHeight: " << rectHeight << std::endl; };
    uint8_t textureId;
    uint32_t rectLeft = 0;
    uint32_t rectTop = 0;
    uint32_t rectWidth = 0;
    uint32_t rectHeight = 0;
};

struct Anime {
    Anime(uint32_t incrementLeft, uint32_t maxWidth, uint32_t targetClock)
        : incrementLeft(incrementLeft)
        , maxWidth(maxWidth)
        , targetClock(targetClock)
    {
    }
    void print() { std::cout << "Anime incrementLeft: " << incrementLeft << " Anime maxWidth: " << maxWidth << " Anime targetClock: " << targetClock << std::endl; };
    uint32_t incrementLeft;
    uint32_t maxWidth;
    uint32_t targetClock;
    uint32_t internClock = 0;
};

struct CollisionGroup {
    CollisionGroup(uint8_t collisionGroup = 0)
        : collisionGroup(collisionGroup) {};
    void print() { std::cout << "CollisionGroup collisionGroup: " <<  static_cast<uint32_t>(collisionGroup) << std::endl; };
    uint8_t collisionGroup;
};
