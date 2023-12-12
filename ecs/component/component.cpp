#include <iostream>

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

struct Velocity {
    Velocity(int velocity = 0, int time = 0, int acc = 0)
        : v(velocity), t(time), a(acc){};
    void print()
    {
        std::cout << "Velocity v: " << v << "\nVelocity t: " << t << "\nVelocity a: " << a << std::endl;
    };
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
