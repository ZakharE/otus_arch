//
// Created by Zakhar Eliseev on 28.03.2022.
//

#ifndef OTUS_STRUCTS_H
#define OTUS_STRUCTS_H
struct Vector2D {
    float x;
    float y;
};

Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs);

bool operator==(const Vector2D &lhs, const Vector2D &rhs);

#endif //OTUS_STRUCTS_H
