#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <iostream>

struct Position {
    Position(int x = 0, int y = 0)
        : x(x)
        , y(y) {};
    void print() { std::cout << "Position x: " << x << "\nPosition y: " << y << std::endl; };
    int x;
    int y;
};

struct Size {
    Size(float width = 0, float height = 0)
        : w(width)
        , h(height) {};
    float w;
    float h;
};

struct Velocity {
    Velocity(int velocity = 0, int time = 0, int acc = 0, int x_speed = 0, int y_speed = 0)
        : x_speed(x_speed)
        , y_speed(y_speed)
        , v(velocity)
        , t(time)
        , a(acc) {};
    void print() { std::cout << "Velocity v: " << v << "\nVelocity t: " << t << "\nVelocity a: " << a << std::endl; };
    int x_speed;
    int y_speed;
    int v;
    int t;
    int a;
};

struct Drawable {
    Drawable(bool drawable = true)
        : isDrawable(drawable) {};
    bool isDrawable;
};

struct Controller {
    Controller(bool controller = true)
        : isController(controller) {};
    bool isController;
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
    uint32_t incrementLeft;
    uint32_t maxWidth;
    uint32_t targetClock;
    uint32_t internClock = 0;
};
