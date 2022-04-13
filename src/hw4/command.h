//
// Created by Zakhar Eliseev on 11.04.2022.
//

#ifndef OTUS_COMMAND_H
#define OTUS_COMMAND_H

#endif //OTUS_COMMAND_H
namespace Command {
    class Command {
    public:
        virtual void execute() = 0;

        virtual ~Command() = default;
    };
}