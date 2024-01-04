/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** IEvent
*/

#ifndef IEVENT_HPP_
#define IEVENT_HPP_

class IEvent {
    public:
        virtual ~IEvent() = default;
        virtual void debug() = 0;
    protected:
    private:
};

#endif /* !IEVENT_HPP_ */
