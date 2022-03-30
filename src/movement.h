//
// Created by Zakhar Eliseev on 28.03.2022.
//

#ifndef OTUS_MOVEMENT_H
#define OTUS_MOVEMENT_H

#include "structs.h"

class IMovable {

public:
    virtual Vector2D get_position() = 0;

    virtual void set_position(Vector2D &&new_position) = 0;

    virtual Vector2D get_velocity() = 0;

    virtual ~IMovable() = default;
};

class LinearMover {
public:
    IMovable *obj;

    explicit LinearMover(IMovable *obj) : obj(obj) {}

    void Execute() {
        obj->set_position(obj->get_position() + obj->get_velocity());
    }
};


#endif //OTUS_MOVEMENT_H
