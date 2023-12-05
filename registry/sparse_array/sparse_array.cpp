/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** sparse_array
*/

#include "sparse_array.hpp"
#include <vector>

template <typename Component>
Sparse_array<Component>::Sparse_array(Sparse_array const &other) : _data(other._data)
{
}

template <typename Component>
Sparse_array<Component> &Sparse_array<Component>::operator=(Sparse_array const &other)
{
    _data = other._data;
    return *this;
}

template <typename Component>
Sparse_array<Component> &Sparse_array<Component>::operator=(Sparse_array &&other) noexcept
{
    _data = std::move(other._data);
    return *this;
}

template <typename Component>
Sparse_array<Component>::reference_type Sparse_array<Component>::operator[](size_t idx)
{
    return _data[idx];
}

template <typename Component>
Sparse_array<Component>::iterator Sparse_array<Component>::begin()
{
    return _data.begin();
}

template <typename Component>
Sparse_array<Component>::const_iterator Sparse_array<Component>::begin() const
{
    return _data.begin();
}

template <typename Component>
Sparse_array<Component>::const_iterator Sparse_array<Component>::cbegin() const
{
    return _data.cbegin();
}

template <typename Component>
Sparse_array<Component>::iterator Sparse_array<Component>::end()
{
    return _data.end();
}

template <typename Component>
Sparse_array<Component>::const_iterator Sparse_array<Component>::end() const
{
    return _data.end();
}

template <typename Component>
Sparse_array<Component>::const_iterator Sparse_array<Component>::cend() const
{
    return _data.cend();
}

template <typename Component>
typename Sparse_array<Component>::size_type Sparse_array<Component>::size() const
{
    return _data.size();
}

template <typename Component>
Sparse_array<Component>::reference_type Sparse_array<Component>::insert_at(size_type pos, Component const &component)
{
    _data.insert(_data.begin() + pos, component);
    return _data[pos];
}

template <typename Component>
Sparse_array<Component>::reference_type Sparse_array<Component>::insert_at(size_type pos, Component &&component)
{
    _data.insert(_data.begin() + pos, std::move(component));
    return _data[pos];
}
