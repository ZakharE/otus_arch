//
// Created by Zakhar Eliseev on 04.04.2022.
//

#ifndef OTUS_COMMAND_H
#define OTUS_COMMAND_H

class Command {
public:
    Command() = default;

    virtual void execute() = 0;
    virtual ~Command() = default;
};

#endif //OTUS_COMMAND_H
