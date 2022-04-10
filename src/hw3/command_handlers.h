//
// Created by Zakhar Eliseev on 04.04.2022.
//

#ifndef OTUS_COMMAND_HANDLERS_H
#define OTUS_COMMAND_HANDLERS_H

#include <iostream>

namespace Command {
    class Base {
    public:
        Base() = default;

        virtual void execute() = 0;

        virtual ~Base() = default;
    };

    template<typename E>
    class LogWriter : public Base {
    public:

        void execute() override {
            std::cout << typeid(E).name() << std::endl;
        }

        ~LogWriter() override = default;
    };

    class Repeater : public Base {
    private:
        std::shared_ptr<Base> command_to_repeat;
    public:
        Repeater(const std::shared_ptr<Base> &c) : command_to_repeat(c) {};

        void execute() override {
            command_to_repeat->execute();
        }

        ~Repeater() override = default;
    };
}
#endif //OTUS_COMMAND_HANDLERS_H
