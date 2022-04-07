//
// Created by Zakhar Eliseev on 30.03.2022.
//
#pragma once

#include "../rotation.h"
#include "gtest/gtest.h"
#include <gmock/gmock.h>

class MockRotator : public IRotable {
public:
    MOCK_METHOD(int, get_direction, (), (override));
    MOCK_METHOD(int, get_angular_velocity, (), (override));
    MOCK_METHOD(void, set_direction, (int), (override));
    MOCK_METHOD(int, get_directions_number, (), (override));
};

TEST(RotatorSuite, RotateObject) {
    testing::NiceMock<MockRotator> mock;
    ON_CALL(mock, get_direction()).WillByDefault(::testing::Return(2));
    ON_CALL(mock, get_angular_velocity()).WillByDefault(::testing::Return(2));
    ON_CALL(mock, get_directions_number()).WillByDefault(::testing::Return(6));
    EXPECT_CALL(mock, set_direction(4));
    Rotator rotator(&mock);
    rotator.Execute();
}