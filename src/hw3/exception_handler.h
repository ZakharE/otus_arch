//
// Created by Zakhar Eliseev on 04.04.2022.
//

#ifndef OTUS_EXCEPTION_HANDLER_H
#define OTUS_EXCEPTION_HANDLER_H

#include <exception>
#include "command.h"
#include <unordered_map>
#include <typeinfo>
#include <queue>

class ExceptionHandler {
public:
    std::unordered_map<std::size_t, std::shared_ptr<Command>> handlers;

    void handle(const Command &c, const std::exception &e);

    void register_handle(const std::shared_ptr<Command> &command, const std::exception &exception,
                         const std::shared_ptr<Command> &handler) {
        size_t combined_hash = get_hash(command, exception);
        handlers[combined_hash] = handler;
    }

    size_t get_hash(const std::shared_ptr<Command> &command, const std::exception &exception) {
        size_t exception_type_hash = typeid(exception).hash_code();
        size_t command_type_hash = typeid(command).hash_code();
        size_t combined_hash = combine_hash(exception_type_hash, command_type_hash);
        return combined_hash;
    };

    void handle(const std::shared_ptr<Command> &c, const std::exception &e);

private:
    std::size_t combine_hash(const std::size_t h1, const std::size_t h2) {
        return h1 ^ (h2 << 1);
    };
};

void ExceptionHandler::handle(const std::shared_ptr<Command> &c, const std::exception &e) {
    size_t combined_hash = get_hash(command, exception);
    handlers[combined_hash]->execute();
}

namespace ErrorHandler {
    class LogWriter {
    private:
        std::queue<std::shared_ptr<Command>> *q;
    public:
        LogWriter(std::queue<std::shared_ptr<Command>> *q) : q(q) {}
    };

    void Execute(const std::shared_ptr<Command> &c) {
        q.push(c);
    }
}
#endif //OTUS_EXCEPTION_HANDLER_H
