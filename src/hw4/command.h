//
// Created by Zakhar Eliseev on 11.04.2022.
//

#ifndef OTUS_COMMAND_H
#define OTUS_COMMAND_H

#include "interfaces.h"
#include <vector>

using std::shared_ptr, std::vector;
namespace Command {
    class Command {
    public:
        virtual void execute() = 0;

        virtual ~Command() = default;
    };

    class MoveCommand : public Command {
    public:
        shared_ptr<IMovable> obj;

        MoveCommand(const shared_ptr<IMovable> &obj) : obj(obj) {}

        void execute() override {
            obj->set_position(obj->get_position() + obj->get_velocity());
        }
    };

    class CheckFuelCommand : public Command {
    public:
        std::shared_ptr<IFuelConsumable> obj;

        CheckFuelCommand(const shared_ptr<IFuelConsumable> &obj) : obj(obj) {}

        void execute() override {
            if (obj->get_remaining_fuel() <= 0) throw std::out_of_range("asd");
        }
    };

    class BurnFuelCommand : public Command {
    public:
        std::shared_ptr<IFuelConsumable> obj;
        float amount;

        explicit BurnFuelCommand(const shared_ptr<IFuelConsumable> &obj, float amount = 0) : obj(obj),
                                                                                             amount(amount) {}

        void execute() override {
            obj->burn_fuel(amount);
        }
    };

    class RotateCommand : public Command {
        std::shared_ptr<IRotable> obj;
    public:
        RotateCommand(const shared_ptr<IRotable> &obj) : obj(obj) {}

        void execute() {
            obj->set_direction((obj->get_direction() + obj->get_angular_velocity()) % obj->get_directions_number());
        }
    };

    class Macro : public Command {
    protected:
        vector<shared_ptr<Command>> command_to_execute;
    public:
        void execute() {
            for (const auto &cmd: command_to_execute) {
                try {
                    cmd->execute();
                } catch (std::exception &e) {
                    throw std::bad_exception();
                }
            }
        }

    public:
        virtual ~Macro() {}
    };

    class LinearMove : public Macro {
    public:
        LinearMove(const shared_ptr<CheckFuelCommand> &checkFuelCommand,
                   const shared_ptr<MoveCommand> &moveCommand,
                   const shared_ptr<BurnFuelCommand> &burnFuelCommand) {
            command_to_execute.push_back(checkFuelCommand);
            command_to_execute.push_back(moveCommand);
            command_to_execute.push_back(burnFuelCommand);
        }

    };
}
#endif //OTUS_COMMAND_H
