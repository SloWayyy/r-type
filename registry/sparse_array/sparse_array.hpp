/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** sparse_array
*/

#ifndef SPARSE_ARRAY_HPP_
#define SPARSE_ARRAY_HPP_

#include <iostream>
#include <optional>
#include <vector>

template <typename Component> // You can also mirror the definition of std :: vector ,

class Sparse_array {
    public:
        using value_type = std::optional<Component>;
        using reference_type = value_type &;
        using const_reference_type = value_type const &;
        using container_t = std::vector<value_type>; // optionally add your allocator
        //templated here.
        using size_type = typename container_t::size_type;
        using iterator = typename container_t::iterator;
        using const_iterator = typename container_t::const_iterator;

    public:
        Sparse_array() = default;                         // You can add more constructors .
        Sparse_array(Sparse_array const &);     // copy constructor
        Sparse_array(Sparse_array &&) = default; // move constructor
        ~Sparse_array() = default;
        Sparse_array &operator=(Sparse_array const &);     // copy assignment operator
        Sparse_array &operator=(Sparse_array &&) noexcept; // move assignment operator
        reference_type operator[](size_t idx);
        const_reference_type operator[](size_t idx) const;
        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;
        iterator end();
        const_iterator end() const;
        const_iterator cend() const;
        size_type size() const;
        size_type size();
        reference_type insert_at(size_type pos, Component const &);
        reference_type insert_at(size_type pos, Component &&);
        template <class... Params>
        reference_type emplace_at(size_type pos, Params &&...); // optional
        void erase(size_type pos);
        size_type get_index(value_type const &) const;
        private:
            container_t _data;
};

#endif /* !SPARSE_ARRAY_HPP_ */
