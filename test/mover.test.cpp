//
// Created by Zakhar Eliseev on 29.03.2022.
//
#pragma once

#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include "../src/movement.h"

class MockObject : public IMovable {
public:
    MOCK_METHOD(Vector2D, get_position, (), (override));
    MOCK_METHOD(Vector2D, get_velocity, (), (override));
    MOCK_METHOD((void), set_position, (Vector2D && ), (override));
};

TEST(MoverTest, MovePlayer) {
    testing::NiceMock<MockObject> mock;
    Vector2D position = {12, 5};
    Vector2D velocity = {-7, 3};
    ON_CALL(mock, get_position()).WillByDefault(::testing::Return(position));
    ON_CALL(mock, get_velocity()).WillByDefault(::testing::Return(velocity));
    EXPECT_CALL(mock, set_position(position + velocity));
    LinearMover mover(&mock);
    mover.Execute();
}