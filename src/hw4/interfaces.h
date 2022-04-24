//
// Created by Zakhar Eliseev on 28.03.2022.
//

#ifndef OTUS_INTERFACES_H
#define OTUS_INTERFACES_H

#include "structs.h"
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

class IRotable {
public:
    virtual int get_direction() = 0;

    virtual int get_angular_velocity() = 0;

    virtual void set_direction(int newV) = 0;

    virtual int get_directions_number() = 0;
};

#endif //OTUS_INTERFACES_H
