//
// Created by Zakhar Eliseev on 12.04.2022.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../movement.h"

class FuelMocK : public IFuelConsumable {
public:
    MOCK_METHOD(float, get_remaining_fuel, (), (override));
    MOCK_METHOD((void), burn_fuel, (float), (override));
};


TEST(Burn_Fuel_Check, If_There_is_Fuel_No_Exception_Thrown) {
    std::shared_ptr<testing::NiceMock<FuelMocK>> fuel_mock = std::make_shared<testing::NiceMock<FuelMocK>>();
    ON_CALL(*fuel_mock, get_remaining_fuel()).WillByDefault(testing::Return(2));
    EXPECT_CALL(*fuel_mock, get_remaining_fuel()).WillOnce(testing::Return(2));
    Command::Command *cmd = new Command::CheckFuelCommand(fuel_mock);
    cmd->execute();
    delete cmd;
};

TEST(Burn_Fuel_Check, If_There_Is_No_Fuel_Exception_Thrown) {
    std::shared_ptr<testing::NiceMock<FuelMocK>> fuel_mock = std::make_shared<testing::NiceMock<FuelMocK>>();
    ON_CALL(*fuel_mock, get_remaining_fuel()).WillByDefault(testing::Return(-2));
    Command::Command *cmd = new Command::CheckFuelCommand(fuel_mock);
    EXPECT_THROW(cmd->execute(), std::out_of_range);
    delete cmd;
};

TEST(A, D) {
    std::shared_ptr<FuelMocK> a = std::make_shared<FuelMocK>();
    std::shared_ptr<FuelMocK> b = std::move(a);
    std::cout << a.get();
}