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
using namespace testing;

class MockThrowableCommand : public Command::Base {
public:
    MOCK_METHOD((void), execute, (), (override));
};

vector<shared_ptr<Command::Base>>
process_queue(const shared_ptr<queue<shared_ptr<Command::Base>>> &q, ExceptionHandlerService &exception_handler) {
    vector<shared_ptr<Command::Base>> executed_command;
    while (!q->empty()) {
        shared_ptr<Command::Base> cmd = q->front();
        q->pop();
        try {
            executed_command.push_back(cmd);
            cmd->execute();
        }
        catch (const exception &ex) {
            exception_handler.handle(cmd, ex);
        }
    }
    return executed_command;
}

TEST(ExceptionHandleSuite, LogwriterCommandTest) {
    //mock setup
    auto mock = make_shared<NiceMock<MockThrowableCommand>>();
    const out_of_range &exc = out_of_range("Range not availible");
    EXPECT_CALL(*mock, execute())
            .Times(1)
            .WillOnce(testing::Throw(exc));
    auto q_ptr = make_shared<queue<shared_ptr<Command::Base>>>();
    q_ptr->push(mock);

    shared_ptr<Command::Base> log_command = make_shared<Command::LogWriter<out_of_range>>();
    auto log_writer_handler = make_shared<CommandExceptionHandler>(q_ptr);
    log_writer_handler->register_handle_command(log_command);

    ExceptionHandlerService exception_handler;
    exception_handler.register_handle(mock, exc, log_writer_handler);

    auto executed_command = process_queue(q_ptr, exception_handler);
    ASSERT_THAT(executed_command, Contains(log_command));
};

TEST(ExceptionHandleSuite, RepeaterCommandTest) {
    auto mock_repeat = make_shared<NiceMock<MockThrowableCommand>>();
    const out_of_range &exc = out_of_range("Range not availible");
    EXPECT_CALL(*mock_repeat, execute())
            .Times(2)
            .WillOnce(testing::Throw(exc))
            .WillRepeatedly(testing::Return());
    auto q_ptr = make_shared<queue<shared_ptr<Command::Base>>>();
    q_ptr->push(mock_repeat);

    ExceptionHandlerService exception_handler;
    shared_ptr<Command::Base> command_to_repeat = make_shared<Command::OneTimeRepeater>(mock_repeat);
    auto repeater_handler = make_shared<CommandExceptionHandler>(q_ptr);
    repeater_handler->register_handle_command(command_to_repeat);
    exception_handler.register_handle(mock_repeat, exc, repeater_handler);

    auto executed_command = process_queue(q_ptr, exception_handler);
    ASSERT_THAT(executed_command, Contains(command_to_repeat));
};

TEST(ExceptionHandleSuite, TwoTimesRepeaterCommandTest) {
    auto mock_repeat = make_shared<NiceMock<MockThrowableCommand>>();
    const out_of_range &exc = out_of_range("Range not availible");

    EXPECT_CALL(*mock_repeat, execute())
            .Times(3)
            .WillOnce(testing::Throw(exc))
            .WillRepeatedly(testing::Return());

    auto q_ptr = make_shared<queue<shared_ptr<Command::Base>>>();
    q_ptr->push(mock_repeat);

    ExceptionHandlerService exception_handler;
    auto command_to_repeat = make_shared<Command::TwoTimeRepeater>(mock_repeat);
    auto repeater_handler = make_shared<CommandExceptionHandler>(q_ptr);
    repeater_handler->register_handle_command(command_to_repeat);
    exception_handler.register_handle(mock_repeat, exc, repeater_handler);

    auto executed_command = process_queue(q_ptr, exception_handler);
    ASSERT_THAT(executed_command, Contains(command_to_repeat));
};



TEST(ExceptionHandleSuite, ComplexCommandTest) {
    auto mock_repeat = make_shared<NiceMock<MockThrowableCommand>>();
    const out_of_range &exc = out_of_range("Range not availible");
    ON_CALL(*mock_repeat, execute())
            .WillByDefault(Throw(exc));

    queue<shared_ptr<Command::Base>> command_queue;
    command_queue.push(mock_repeat);
    auto q_ptr = make_shared<queue<shared_ptr<Command::Base>>>(command_queue);

    ExceptionHandlerService exception_handler;
    shared_ptr<Command::Base> repeat_command = make_shared<Command::OneTimeRepeater>(mock_repeat);
    auto complex_handler = make_shared<CommandExceptionHandler>(q_ptr);
    complex_handler->register_handle_command(repeat_command);

    shared_ptr<Command::Base> log_command = make_shared<Command::LogWriter<out_of_range>>();
    complex_handler->register_handle_command(log_command);

    exception_handler.register_handle(mock_repeat, exc, complex_handler);

    auto executed_command = process_queue(q_ptr, exception_handler);
    EXPECT_THAT(executed_command, AllOf(Contains(repeat_command), Contains(log_command)));
};