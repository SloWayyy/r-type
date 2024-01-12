/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** loggerSystem
*/

#ifndef LOGGERSYSTEM_HPP_
#define LOGGERSYSTEM_HPP_

#include "../../ecs/system/ISystem.hpp"
#include "../../ecs/registry/registry.hpp"

enum Component {
    DEFAULT,
    POSITION,
    VELOCITY,
    SIZE,
    SPRITE,
    COLLISION,
    HITBOX,
    LAST
};

class LoggerSystem : public ISystem {
    public:
        LoggerSystem() = delete;
        LoggerSystem(registry &reg): _reg(reg) {};
        ~LoggerSystem() = default;
        void operator()() override {
            checkInput();
            if (_target == _lastTarget && _targetComponent == _lastTargetComponent)
                return;
            _lastTarget = _target;
            _lastTargetComponent = _targetComponent;
            displayComponent();
        };
    private:

        void checkInput() {
            if (_reg._eventManager.checkEvent<keyPressed>()) {
                for (auto &tmp : _reg._eventManager.getEvent<keyPressed>()) {
                    if (tmp->_key == sf::Keyboard::Down) {
                        if (_targetComponent < LAST - 1)
                            _targetComponent++;
                    }
                    if (tmp->_key == sf::Keyboard::Up) {
                        if (_targetComponent > 0)
                            _targetComponent--;
                    }
                    if (tmp->_key == sf::Keyboard::Right) {
                        if (_target < DEFAULT_SIZE - 1)
                            _target++;
                    }
                    if (tmp->_key == sf::Keyboard::Left) {
                        if (_target > 0)
                            _target--;
                    }
                }
            }
            if (_reg._eventManager.checkEvent<textEntered>()) {
                for (auto &tmp : _reg._eventManager.getEvent<textEntered>()) {
                    if (tmp->_key == '2') {
                        if (_targetComponent < LAST - 1)
                            _targetComponent++;
                    }
                    if (tmp->_key == '8') {
                        if (_targetComponent > 0)
                            _targetComponent--;
                    }
                    if (tmp->_key == '6') {
                        if (_target < DEFAULT_SIZE - 1)
                            _target++;
                    }
                    if (tmp->_key == '4') {
                        if (_target > 0)
                            _target--;
                    }
                }
            }
        }

        void displayComponent() {
            switch (_targetComponent) {
                case POSITION:
                    if (position[_target]) {
                        std::cout << "entity [" << _target << "] ";
                        position[_target].value().print();
                    } else {
                        std::cout << "entity [" << _target << "]" << std::endl;
                    }
                    break;

                case VELOCITY:
                    if (velocity[_target]) {
                        std::cout << "entity [" << _target << "] ";
                        velocity[_target].value().print();
                    } else {
                        std::cout << "entity [" << _target << "]" << std::endl;
                    }
                    break;

                case SIZE:
                    if (size[_target]) {
                        std::cout << "entity [" << _target << "] ";
                        size[_target].value().print();
                    } else {
                        std::cout << "entity [" << _target << "]" << std::endl;
                    }
                    break;

                case SPRITE:
                    if (sprite[_target]) {
                        std::cout << "entity [" << _target << "] ";
                        sprite[_target].value().print();
                    } else {
                        std::cout << "entity [" << _target << "]" << std::endl;
                    }
                    break;

                case COLLISION:
                    if (collision[_target]) {
                        std::cout << "entity [" << _target << "] ";
                        collision[_target].value().print();
                    } else {
                        std::cout << "entity [" << _target << "]" << std::endl;
                    }
                    break;

                case HITBOX:
                    if (hitbox[_target]) {
                        std::cout << "entity [" << _target << "] ";
                        hitbox[_target].value().print();
                    } else {
                        std::cout << "entity [" << _target << "]" << std::endl;
                    }
                    break;
                
                default:
                    std::cout << std::endl;
                    break;
            }
        }


        registry &_reg;
        uint32_t _target = 0;
        uint32_t _lastTarget = 0;
        uint32_t _targetComponent = DEFAULT;
        uint32_t _lastTargetComponent = DEFAULT;
        Sparse_array<Position> &position = _reg.getComponent<Position>();
        Sparse_array<Velocity> &velocity = _reg.getComponent<Velocity>();
        Sparse_array<Size> &size = _reg.getComponent<Size>();
        Sparse_array<Sprite> &sprite = _reg.getComponent<Sprite>();
        Sparse_array<CollisionGroup> &collision = _reg.getComponent<CollisionGroup>();
        Sparse_array<HitBox> &hitbox = _reg.getComponent<HitBox>();
};

#endif /* !LOGGERSYSTEM_HPP_ */
