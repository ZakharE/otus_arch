//
// Created by Zakhar Eliseev on 30.03.2022.
//

#ifndef OTUS_ROTATION_H
#define OTUS_ROTATION_H


class IRotable {
public:
    virtual int get_direction() = 0;

    virtual int get_angular_velocity() = 0;

    virtual void set_direction(int newV) = 0;

    virtual int get_directions_number() = 0;
};

class Rotator {
    IRotable *obj;
public:
    explicit Rotator(IRotable *obj) : obj(obj) {}

    void Execute() {
        obj->set_direction((obj->get_direction() + obj->get_angular_velocity()) % obj->get_directions_number());
    }
};

#endif //OTUS_ROTATION_H
