//
// Created by Zakhar Eliseev on 28.03.2022.
//

#ifndef OTUS_MOVEMENT_H
#define OTUS_MOVEMENT_H

#include "structs.h"
#include "command.h"
#include <exception>
#include <utility>

class IMovable {

public:
    virtual Vector2D get_position() = 0;

    virtual void set_position(Vector2D &&new_position) = 0;

    virtual Vector2D get_velocity() = 0;

    virtual ~IMovable() = default;
};

class IFuelConsumable {

public:
    virtual float get_remaining_fuel() = 0;

    virtual void burn_fuel(float amount) = 0;

    virtual ~IFuelConsumable() = default;
};

namespace Command {
    class MoveCommand : public Command {
    public:
        IMovable *obj;

        explicit MoveCommand(IMovable *obj) : obj(obj) {}

        void execute() override {
            obj->set_position(obj->get_position() + obj->get_velocity());
        }
    };

    class CheckFuelCommand : public Command {
    public:
        std::shared_ptr<IFuelConsumable> obj;

        CheckFuelCommand(const std::shared_ptr<IFuelConsumable> &obj) : obj(obj) {}

        void execute() override {
            if (obj->get_remaining_fuel() <= 0) throw std::out_of_range("asd");
        }
    };

    class BurnFuelCommand : public Command {
    public:
        IFuelConsumable *obj;
        float amount;

        explicit BurnFuelCommand(IFuelConsumable *obj, float amount = 0) : obj(obj), amount(amount) {}

        void execute() override {
            obj->burn_fuel(amount);
        }
    };
}


#endif //OTUS_MOVEMENT_H
