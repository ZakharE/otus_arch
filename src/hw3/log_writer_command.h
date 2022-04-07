//
// Created by Zakhar Eliseev on 04.04.2022.
//

#ifndef OTUS_LOG_WRITER_COMMAND_H
#define OTUS_LOG_WRITER_COMMAND_H

#include "command.h"
#include <iostream>

template<typename E>
class LogWriterCommand : public Command {
public:

    void execute() override {
        std::cout << typeid(E).name() << std::endl;
    }

    ~LogWriterCommand() override = default;
};

#endif //OTUS_LOG_WRITER_COMMAND_H
