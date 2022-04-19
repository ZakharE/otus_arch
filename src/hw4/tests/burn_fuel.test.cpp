//
// Created by Zakhar Eliseev on 12.04.2022.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../interfaces.h"
#include "../command.h"

using testing::_;

class FuelMocK : public IFuelConsumable, public IRotable, public IMovable {
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


TEST(Check_Fuel, If_There_is_Fuel_No_Exception_Thrown) {
    std::shared_ptr<testing::NiceMock<FuelMocK>> fuel_mock = std::make_shared<testing::NiceMock<FuelMocK>>();
    EXPECT_CALL(*fuel_mock, get_remaining_fuel()).WillOnce(testing::Return(2));
    Command::Command *cmd = new Command::CheckFuelCommand(fuel_mock);
    cmd->execute();
    delete cmd;
};

TEST(Check_Fuel, If_There_Is_No_Fuel_Exception_Thrown) {
    std::shared_ptr<testing::NiceMock<FuelMocK>> fuel_mock = std::make_shared<testing::NiceMock<FuelMocK>>();
    ON_CALL(*fuel_mock, get_remaining_fuel()).WillByDefault(testing::Return(-2));
    Command::Command *cmd = new Command::CheckFuelCommand(fuel_mock);
    EXPECT_THROW(cmd->execute(), std::out_of_range);
    delete cmd;
};

TEST(Burn_Fuel, if_amount_not_set_use_zero) {
    std::shared_ptr<testing::NiceMock<FuelMocK>> fuel_mock = std::make_shared<testing::NiceMock<FuelMocK>>();
    ON_CALL(*fuel_mock, burn_fuel(testing::_))
            .WillByDefault(testing::Return());
    EXPECT_CALL(*fuel_mock, burn_fuel(0));
    Command::Command *cmd = new Command::BurnFuelCommand(fuel_mock);
    cmd->execute();
    delete cmd;
};

TEST(Burn_Fuel, fuel_ampunt_will_burn_if_value_non_zero) {
    std::shared_ptr<testing::NiceMock<FuelMocK>> fuel_mock = std::make_shared<testing::NiceMock<FuelMocK>>();
    ON_CALL(*fuel_mock, burn_fuel(testing::_))
            .WillByDefault(testing::Return());
    EXPECT_CALL(*fuel_mock, burn_fuel(2));
    Command::Command *cmd = new Command::BurnFuelCommand(fuel_mock, 2);
    cmd->execute();
    delete cmd;
};

TEST(Macro_Command, Liner_Move_Without_Exception) {
    std::shared_ptr<FuelMocK> fuel_mock = std::make_shared<testing::NiceMock<FuelMocK>>();


    auto check_fuel_command = std::make_shared<Command::CheckFuelCommand>(fuel_mock);
    EXPECT_CALL(*fuel_mock, get_remaining_fuel()).WillOnce(::testing::Return(2));

    auto burn_fuel_command = std::make_shared<Command::BurnFuelCommand>(fuel_mock, 2);
    EXPECT_CALL(*fuel_mock, burn_fuel(2));


    auto move_command = std::make_shared<Command::MoveCommand>(fuel_mock);
    Vector2D position = {12, 5};
    Vector2D velocity = {-7, 3};
    ON_CALL(*fuel_mock, get_position()).WillByDefault(::testing::Return(position));
    ON_CALL(*fuel_mock, get_velocity()).WillByDefault(::testing::Return(velocity));
    EXPECT_CALL(*fuel_mock, set_position(position + velocity));
    auto linear_move_command = std::make_shared<Command::LinearMove>(check_fuel_command, move_command,
                                                                     burn_fuel_command);
    linear_move_command->execute();
}

TEST(Macro_Command, If_Not_Enough_Fuel_Exception_Will_Thrown) {
    std::shared_ptr<FuelMocK> fuel_mock = std::make_shared<testing::NiceMock<FuelMocK>>();
    auto check_fuel_command = std::make_shared<Command::CheckFuelCommand>(fuel_mock);
    EXPECT_CALL(*fuel_mock, get_remaining_fuel()).WillOnce(::testing::Return(0));
    auto burn_fuel_command = std::make_shared<Command::BurnFuelCommand>(fuel_mock, 2);
    EXPECT_CALL(*fuel_mock, burn_fuel(_)).Times(0);
    auto move_command = std::make_shared<Command::MoveCommand>(fuel_mock);
    EXPECT_CALL(*fuel_mock, set_position(_)).Times(0);
    auto linear_move_command = std::make_shared<Command::LinearMove>(check_fuel_command, move_command,
                                                                     burn_fuel_command);
    EXPECT_ANY_THROW(linear_move_command->execute());
}