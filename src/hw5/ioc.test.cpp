#include "gtest/gtest.h"
#include "ioc.h"
#include "../test_shared/mock.h"
#include <thread>

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

TEST(Ioc_container, register_new_scope) {
    std::string command_name = "Move forward";
    std::shared_ptr<Mock> fuel_mock = std::make_shared<testing::NiceMock<Mock>>();


    auto check_fuel_command = std::make_shared<Command::CheckFuelCommand>(fuel_mock);
    auto burn_fuel_command = std::make_shared<Command::BurnFuelCommand>(fuel_mock, 2);
    auto move_command = std::make_shared<Command::MoveCommand>(fuel_mock);
    auto rotate_command = std::make_shared<Command::RotateCommand>(fuel_mock);

    EXPECT_CALL(*fuel_mock, get_remaining_fuel()).WillOnce(::testing::Return(2));
    EXPECT_CALL(*fuel_mock, burn_fuel(2));
    Vector2D position = {12, 5};
    Vector2D velocity = {-7, 3};
    ON_CALL(*fuel_mock, get_position()).WillByDefault(::testing::Return(position));
    ON_CALL(*fuel_mock, get_velocity()).WillByDefault(::testing::Return(velocity));
    ON_CALL(*fuel_mock, get_direction()).WillByDefault(::testing::Return(2));
    ON_CALL(*fuel_mock, get_directions_number()).WillByDefault(::testing::Return(3));
    ON_CALL(*fuel_mock, get_angular_velocity()).WillByDefault(::testing::Return(4));
    EXPECT_CALL(*fuel_mock, set_position(position + velocity));
    EXPECT_CALL(*fuel_mock, set_direction(::testing::_));
    auto linear_move_command = std::make_shared<Command::LinearMove>(check_fuel_command, move_command,
                                                                     burn_fuel_command);

    Function linear_move_command_f = [&](const Args &args) {
        return linear_move_command;
    };

    Function rotate_command_f = [&](const Args &args) {
        return rotate_command;
    };

    Container c;
    c.init();
    std::vector<std::any> args{"player 1"};
    auto player1_context = c.resolve<std::shared_ptr<Container>>("Scope.New", args);
    args = {linear_move_command_f};
    player1_context->resolve<std::shared_ptr<Container::RegisterCommand>>("Move forward", args)->execute();

    args = {"player 2"};
    auto player2_context = c.resolve<std::shared_ptr<Container>>("Scope.New", args);
    args = {rotate_command_f};
    player2_context->resolve<std::shared_ptr<Container::RegisterCommand>>("Rotate", args)->execute();

    args.clear(); args.emplace_back(1);
    player1_context->resolve<std::shared_ptr<Command::LinearMove>>("Move forward", args)->execute();
    args = {};
    player2_context->resolve<std::shared_ptr<Command::RotateCommand>>("Rotate", args)->execute();
}