/*
** EPITECH PROJECT, 2023
** R-type
** File description:
** ISystem
*/

#ifndef ISYSTEM_HPP_
#define ISYSTEM_HPP_

class ISystem {
    public:
        virtual ~ISystem() = default;
        virtual void operator()() = 0;
    protected:
    private:
};

#endif /* !ISYSTEM_HPP_ */
