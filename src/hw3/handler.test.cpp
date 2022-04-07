//
// Created by Zakhar Eliseev on 04.04.2022.
//
#include "gtest/gtest.h"
#include "command.h"
#include "log_writer_command.h"
#include "exception_handler.h"
#include <gmock/gmock.h>
#include <queue>
#include <memory>

using namespace std;

class MockThrowableCommand : public Command {
public:
    MOCK_METHOD((void), execute, (), (override));
};

TEST(ExceptionHandleSuite, LogwriterCommandTest) {
    //mock setup
    auto mock = make_shared<::testing::NiceMock<MockThrowableCommand>>();
    const out_of_range &exc = out_of_range("Range not availible");
    ON_CALL(*mock, execute()).WillByDefault(::testing::Throw(exc));
    queue<shared_ptr<Command>> command_queue;
    command_queue.push(mock);
    ExceptionHandler exception_handler();
    auto log = make_shared<LogWriterCommand<out_of_range>>();
    exception_handler
            .register_handle(mock, exc, log);
    while (!command_queue.empty()) {
        std::shared_ptr<Command> cmd = command_queue.front();
        command_queue.pop();

        try {
            cmd->execute();
        }
        catch (const std::exception &ex) {
            exception_handler.handle(cmd, ex);
        }
    }
}

TEST(A,B) {

}