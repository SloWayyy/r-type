#include "registry/registry.hpp"
#include "registry/sparse_array/sparse_array.hpp"



struct Position
{
    int x;
    int y;
};

int main()
{
    
    registry reg;
    Sparse_array<Position> pos;
    reg.add_component(&pos);
    auto test = std::any_cast<Sparse_array<Position>>(reg._component_arrays[std::type_index(typeid(pos))]);
    std::cout << sizeof(test) << std::endl;
    return 0;
}