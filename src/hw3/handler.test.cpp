//
// Created by Zakhar Eliseev on 04.04.2022.
//
#include "gtest/gtest.h"
#include "command_handlers.h"
#include "exception_handler.h"
#include <gmock/gmock.h>
#include <queue>
#include <memory>

using namespace std;

class MockThrowableCommand : public Command::Base {
public:
    MOCK_METHOD((void), execute, (), (override));
};

TEST(ExceptionHandleSuite, LogwriterCommandTest) {
    //mock setup
    auto mock = make_shared<::testing::NiceMock<MockThrowableCommand>>();
    const out_of_range &exc = out_of_range("Range not availible");
    ON_CALL(*mock, execute()).WillByDefault(::testing::Throw(exc));

    auto q_ptr = make_shared<queue<shared_ptr<Command::Base>>>();
    q_ptr->push(mock);

    shared_ptr<Command::Base> log_command = make_shared<Command::LogWriter<out_of_range>>();
    auto log_writer_handler = make_shared<CommandExceptionHandler>(q_ptr);
    log_writer_handler->register_handle_command(log_command);

    ExceptionHandlerService exception_handler;
    exception_handler.register_handle(mock, exc, log_writer_handler);
    std::shared_ptr<Command::Base> cmd = q_ptr->front();
    q_ptr->pop();

    try {
        cmd->execute();
    }
    catch (const std::exception &ex) {
        exception_handler.handle(cmd, ex);
    }

    EXPECT_THAT(q_ptr->size(), ::testing::Eq(1));
    EXPECT_THAT(q_ptr->front(), ::testing::Eq(log_command));
};

TEST(ExceptionHandleSuite, RepeaterCommandTest) {
    auto mock_repeat = make_shared<::testing::NiceMock<MockThrowableCommand>>();
    const out_of_range &exc = out_of_range("Range not availible");
    ON_CALL(*mock_repeat, execute()).WillByDefault(::testing::Throw(exc));

    auto q_ptr = make_shared<queue<shared_ptr<Command::Base>>>();
    q_ptr->push(mock_repeat);

    ExceptionHandlerService exception_handler;
    shared_ptr<Command::Base> command_to_repeat = make_shared<Command::Repeater>(mock_repeat);
    auto repeater_handler = make_shared<CommandExceptionHandler>(q_ptr);
    repeater_handler->register_handle_command(command_to_repeat);
    exception_handler.register_handle(mock_repeat, exc, repeater_handler);

    std::shared_ptr<Command::Base> cmd = q_ptr->front();
    q_ptr->pop();

    try {
        cmd->execute();
    }
    catch (const std::exception &ex) {
        exception_handler.handle(cmd, ex);
    }
    EXPECT_THAT(q_ptr->size(), ::testing::Eq(1));
    EXPECT_THAT(q_ptr->front(), ::testing::Eq(command_to_repeat));
};

TEST(ExceptionHandleSuite, ComplexCommandTest) {
    auto mock_repeat = make_shared<::testing::NiceMock<MockThrowableCommand>>();
    const out_of_range &exc = out_of_range("Range not availible");
    ON_CALL(*mock_repeat, execute()).WillByDefault(::testing::Throw(exc));

    queue<shared_ptr<Command::Base>> command_queue;
    command_queue.push(mock_repeat);
    auto q_ptr = make_shared<queue<shared_ptr<Command::Base>>>(command_queue);

    ExceptionHandlerService exception_handler;
    shared_ptr<Command::Base> repeat_command = make_shared<Command::Repeater>(mock_repeat);
    auto complex_handler = make_shared<CommandExceptionHandler>(q_ptr);
    complex_handler->register_handle_command(repeat_command);

    shared_ptr<Command::Base> log_command = make_shared<Command::LogWriter<out_of_range>>();
    complex_handler->register_handle_command(log_command);

    exception_handler.register_handle(mock_repeat, exc, complex_handler);

    vector<shared_ptr<Command::Base>> added_commands;
    while (!q_ptr->empty()) {
        auto command = q_ptr->front();
        q_ptr->pop();
        try {
            command->execute();
        }
        catch (const std::exception &ex) {
            exception_handler.handle(command, ex);
            added_commands.push_back(q_ptr->front());
        }
    }

    EXPECT_THAT(added_commands.size(), ::testing::Eq(2));
    EXPECT_THAT(added_commands, ::testing::ElementsAre(repeat_command, log_command));
};