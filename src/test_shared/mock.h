//
// Created by Zakhar Eliseev on 29.04.2022.
//

#ifndef OTUS_MOCK_H
#define OTUS_MOCK_H

#include "gmock/gmock.h"

class Mock : public IFuelConsumable, public IRotable, public IMovable {
public:
    MOCK_METHOD(float, get_remaining_fuel, (), (override));
    MOCK_METHOD((void), burn_fuel, (float), (override));
    MOCK_METHOD(int, get_direction, (), (override));
    MOCK_METHOD(int, get_angular_velocity, (), (override));
    MOCK_METHOD(void, set_direction, (int), (override));
    MOCK_METHOD(int, get_directions_number, (), (override));
    MOCK_METHOD(Vector2D, get_position, (), (override));
    MOCK_METHOD(Vector2D, get_velocity, (), (override));
    MOCK_METHOD((void), set_position, (Vector2D && ), (override));
};

#endif //OTUS_MOCK_H
