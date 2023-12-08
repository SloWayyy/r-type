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
        Sparse_array(Sparse_array const &other): _data(other._data) {}; // copy constructor
        Sparse_array(Sparse_array &&) = default; // move constructor
        ~Sparse_array() = default;
        Sparse_array &operator=(Sparse_array const &other) {
            _data = other._data;
            return *this;
        };
        Sparse_array &operator=(Sparse_array &&other) noexcept {
            _data = std::move(other._data);
            return *this;
        };
        reference_type operator[](size_t idx) {
            return _data[idx];
        };
        const_reference_type operator[](size_t idx) const {
            return _data[idx];
        };
        iterator begin() {
            return _data.begin();
        };
        const_iterator begin() const {
            return _data.begin();
        };
        const_iterator cbegin() const {
            return _data.cbegin();
        };
        iterator end() {
            return _data.end();
        };
        const_iterator end() const {
            return _data.end();
        };
        const_iterator cend() const {
            return _data.cend();
        };
        size_type size() const {
            return _data.size();
        };
        size_type size() {
            return _data.size();
        };

        reference_type push_back(value_type const &component = value_type()) {
            _data.push_back(component);
            return _data.back();
        };

        reference_type insert_at(size_type pos, Component const &component) {
            _data.insert(_data.begin() + pos, component);
            return _data[pos];
        };
        reference_type insert_at(size_type pos, Component &&component) {
            _data.insert(_data.begin() + pos, std::forward<Component>(component));
            return _data[pos];
        };

        template <class... Params>
        reference_type emplace_at(size_type pos, Params &&... params) {
            Component component(std::forward<Params>(params)...);
            _data.insert(_data.begin() + pos, std::move(component));
            return _data[pos];
        }; // optional
        void erase(size_type pos) {
            _data.erase(_data.begin() + pos);
        };
        size_type get_index(value_type const &component) const {
            for (size_type i = 0; i < _data.size(); i++) {
                if (_data[i] == component)
                    return i;
            }
            return -1;
        };
        private:
            container_t _data;
};


#endif /* !SPARSE_ARRAY_HPP_ */
