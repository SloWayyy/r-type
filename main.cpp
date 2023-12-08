#include "registry/registry.hpp"
#include "registry/sparse_array/sparse_array.hpp"



struct Position
{
    Position(int x, int y) : x(x), y(y) {};
    int x;
    int y;
};

int main()
{
    
    registry reg;
    reg.addComponent<Position>();
    std::cout << reg.getComponent<Position>().size() << std::endl;
    reg.getComponent<Position>().emplace_at(0, 1, 2);
    std::cout << reg.getComponent<Position>().size() << std::endl;
    return 0;

}
