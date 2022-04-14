//
// Created by Zakhar Eliseev on 04.04.2022.
//

#ifndef OTUS_EXCEPTION_HANDLER_H
#define OTUS_EXCEPTION_HANDLER_H

#include <exception>
#include <typeinfo>
#include <unordered_map>
#include <queue>

class CommandExceptionHandler {
private:
    std::shared_ptr<std::queue<std::shared_ptr<Command::Base>>> command_queue;
    std::queue<std::shared_ptr<Command::Base>> command_to_add_queue;
public:
    CommandExceptionHandler(std::shared_ptr<std::queue<std::shared_ptr<Command::Base>>> &q)
            : command_queue(q) {}

    void execute() {
        command_queue->push(command_to_add_queue.front());
        command_to_add_queue.pop();
    };

    void register_handle_command(const std::shared_ptr<Command::Base> &c) {
        command_to_add_queue.push(c);
    }
};


class ExceptionHandlerService {
public:
    std::unordered_map<std::size_t, std::shared_ptr<CommandExceptionHandler>> handlers;

    void register_handle(const std::shared_ptr<Command::Base> &command,
                         const std::exception &exception,
                         const std::shared_ptr<CommandExceptionHandler> &handler) {
        size_t combined_hash = get_hash(command, exception);
        handlers[combined_hash] = handler;
    };

    void handle(const std::shared_ptr<Command::Base> &c, const std::exception &e) {
        size_t combined_hash = get_hash(c, e);
        if (handlers.count(combined_hash)) {
            handlers[combined_hash]->execute();
        }
    };

private:
    size_t get_hash(const std::shared_ptr<Command::Base> &command, const std::exception &exception) {
        size_t exception_type_hash = typeid(exception).hash_code();
        size_t command_type_hash = typeid(command).hash_code();
        size_t combined_hash = combine_hash(exception_type_hash, command_type_hash);
        return combined_hash;
    };

    std::size_t combine_hash(const std::size_t h1, const std::size_t h2) {
        return h1 ^ (h2 << 1);
    };
};

#endif //OTUS_EXCEPTION_HANDLER_H
