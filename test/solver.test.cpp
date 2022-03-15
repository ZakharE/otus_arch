//
// Created by Zakhar Eliseev on 13.03.2022.
//
#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include <vector>
#include "../src/solver.h"

TEST(RootTests, HandleImagenaryRoots) {
    EXPECT_THAT(Math::solve(1, 0, 1), ::testing::IsEmpty()) << "There are some roots, but descriminant < 0";
}

TEST(RootTests, HandleTwoRoots) {
    EXPECT_THAT(Math::solve(1, 0, -1), ::testing::ElementsAreArray({1, -1})) << "Wrong roots!";
}

TEST(RootTests, HandleTwoSameRoots) {
    EXPECT_THAT(Math::solve(1, -2, 1), ::testing::ElementsAreArray({1, 1})) << "Wrong roots!";
}

TEST(RootTests, HandleLinearEquation) {
    EXPECT_THROW(Math::solve(0, -2, 1), std::invalid_argument);
}

TEST(RootTests, HandleDiscriminantClosseToZero) {
    EXPECT_THAT(Math::solve(0.99999, -2, 0.99999),
                ::testing::ElementsAreArray(
                        {testing::DoubleNear(1.00001, 0.001), testing::DoubleNear(1.00001, 0.001)}))
                        << "Wrong roots or roots calculated with wrong precision";
}


//parametrized tests for non-numeric values
class RootTestFixture : public ::testing::TestWithParam<double> {
};

INSTANTIATE_TEST_SUITE_P(RootTests, RootTestFixture, ::testing::Values(NAN, INFINITY));

TEST_P(RootTestFixture, HandleNonNumericValueFirstArgument) {
    EXPECT_THROW(Math::solve(GetParam(), -2, 1), std::invalid_argument)
                        << "No exception was thrown when 'a' is " << GetParam();
}

TEST_P(RootTestFixture, HandleNonNumericValueSecondArgument) {
    EXPECT_THROW(Math::solve(1, GetParam(), 1), std::invalid_argument)
                        << "No exception was thrown when 'b' is " << GetParam();
}

TEST_P(RootTestFixture, HandleNonNumericValueThirdArgument) {
    EXPECT_THROW(Math::solve(1, 1, GetParam()), std::invalid_argument)
                        << "No exception was thrown when 'c' is " << GetParam();;
}
