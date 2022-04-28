//
// Created by Zakhar Eliseev on 29.04.2022.
//
#include "structs.h"

Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs) {
    return Vector2D{lhs.x + rhs.x, lhs.y + rhs.y};
}

bool operator==(const Vector2D &lhs, const Vector2D &rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
