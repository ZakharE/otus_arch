#include "gtest/gtest.h"
#include "ioc.h"
#include "../test_shared/mock.h"

TEST(IoC_container, CanResolveRegistredDependency) {
    std::string command_name = "Move forward";
    std::shared_ptr<Mock> fuel_mock = std::make_shared<testing::NiceMock<Mock>>();


    auto check_fuel_command = std::make_shared<Command::CheckFuelCommand>(fuel_mock);
    auto burn_fuel_command = std::make_shared<Command::BurnFuelCommand>(fuel_mock, 2);
    auto move_command = std::make_shared<Command::MoveCommand>(fuel_mock);

    EXPECT_CALL(*fuel_mock, get_remaining_fuel()).WillOnce(::testing::Return(2));
    EXPECT_CALL(*fuel_mock, burn_fuel(2));
    Vector2D position = {12, 5};
    Vector2D velocity = {-7, 3};
    ON_CALL(*fuel_mock, get_position()).WillByDefault(::testing::Return(position));
    ON_CALL(*fuel_mock, get_velocity()).WillByDefault(::testing::Return(velocity));
    EXPECT_CALL(*fuel_mock, set_position(position + velocity));
    auto linear_move_command = std::make_shared<Command::LinearMove>(check_fuel_command, move_command,
                                                                     burn_fuel_command);

    Function f = [&](const Args &args) {
        return linear_move_command;
    };

    Container c;
    c.init();
    std::vector<std::any> args = {f};
    c.resolve<std::shared_ptr<Container::RegisterCommand>>(command_name, args)->execute();
    args.clear();
    args.emplace_back(1);
    c.resolve<std::shared_ptr<Command::LinearMove>>(command_name, args)->execute();
}

TEST(IoC_container, ExceptionIsThrown_IfCalledNonRegistredDependency) {
    Container c;
    c.init();
    std::vector<std::any> args = {1, 1, 1};
    EXPECT_THROW(c.resolve<int>("no.such.dependency", args), std::invalid_argument);
}

TEST(IoC_container, ExceptionIsThrown_IfCalledNonRegistredDependency) {
    Container c;
    c.init();
    std::vector<std::any> args = {1, 1, 1};
    EXPECT_THROW(c.resolve<int>("no.such.dependency", args), std::invalid_argument);
}