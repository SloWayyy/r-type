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
typename Sparse_array<Component>::reference_type Sparse_array<Component>::operator[](size_t idx)
{
    return _data[idx];
}

template <typename Component>
typename Sparse_array<Component>::const_reference_type Sparse_array<Component>::operator[](size_t idx) const
{
    return _data[idx];
}


template <typename Component>
typename Sparse_array<Component>::iterator Sparse_array<Component>::begin()
{
    return _data.begin();
}

template <typename Component>
typename Sparse_array<Component>::const_iterator Sparse_array<Component>::begin() const
{
    return _data.begin();
}

template <typename Component>
typename Sparse_array<Component>::const_iterator Sparse_array<Component>::cbegin() const
{
    return _data.cbegin();
}

template <typename Component>
typename Sparse_array<Component>::iterator Sparse_array<Component>::end()
{
    return _data.end();
}

template <typename Component>
typename Sparse_array<Component>::const_iterator Sparse_array<Component>::end() const
{
    return _data.end();
}

template <typename Component>
typename Sparse_array<Component>::const_iterator Sparse_array<Component>::cend() const
{
    return _data.cend();
}

template <typename Component>
typename Sparse_array<Component>::size_type Sparse_array<Component>::size() const
{
    return _data.size();
}

template <typename Component>
typename Sparse_array<Component>::size_type Sparse_array<Component>::size()
{
    return _data.size();
}

template <typename Component>
typename Sparse_array<Component>::reference_type Sparse_array<Component>::insert_at(size_type pos, Component const &component)
{
    _data.insert_at(_data.begin() + pos, component);
    return _data[pos];
}

template <typename Component>
typename Sparse_array<Component>::reference_type Sparse_array<Component>::insert_at(size_type pos, Component &&component)
{
    _data.insert_at(_data.begin() + pos, std::forward<Component>(component));
    return _data[pos];
}

template <typename Component>
template <class ...Params>
typename Sparse_array<Component>::reference_type Sparse_array<Component>::emplace_at(size_type pos, Params &&...params)
{
    Component component(std::forward<Params>(params)...);

    _data.insert_at(_data.begin() + pos, std::move(component));
    return _data[pos];
}

template <typename Component>
void Sparse_array<Component>::erase(size_type pos)
{
    _data.erase(_data.begin() + pos);
}

template <typename Component>
typename Sparse_array<Component>::size_type Sparse_array<Component>::get_index(value_type const &component) const
{
    for (size_type i = 0; i < _data.size(); i++) {
        if (_data[i] == component)
            return i;
    }
    return -1;
}
