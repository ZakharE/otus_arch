//
// Created by Zakhar Eliseev on 13.03.2022.
//
#include "gtest/gtest.h"
#include "../func.h"
TEST(FactorialTest, HandlesZeroInput) {
    ASSERT_TRUE(is_even(2)) << "Not even!";
}