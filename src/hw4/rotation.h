//
// Created by Zakhar Eliseev on 30.03.2022.
//

#ifndef OTUS_ROTATION_H
#define OTUS_ROTATION_H

#include "command.h"

class IRotable {
public:
    virtual int get_direction() = 0;

    virtual int get_angular_velocity() = 0;

    virtual void set_direction(int newV) = 0;

    virtual int get_directions_number() = 0;
};
namespace Command {
    class RotateCommand {
        IRotable *obj;
    public:
        explicit RotateCommand(IRotable *obj) : obj(obj) {}

        void Execute() {
            obj->set_direction((obj->get_direction() + obj->get_angular_velocity()) % obj->get_directions_number());
        }
    };
}

#endif //OTUS_ROTATION_H
