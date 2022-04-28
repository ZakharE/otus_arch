//
// Created by Zakhar Eliseev on 24.04.2022.
//

#ifndef OTUS_IOC_H
#define OTUS_IOC_H

#include <any>
#include <tuple>
#include <functional>
#include <string>
#include "../hw4/command.h"

#include <string>
#include <iostream>

using Args = std::vector<std::any>;

using Function = std::function<std::any(Args)>;
using MapId2Lambda = std::map<std::string, Function>;

template<typename T>
T get_required_arg(Args &args, const std::size_t &idx, const std::string &error) {
    std::size_t args_size = args.size();
    if (idx < 0 || idx > args_size) {
        throw std::invalid_argument(error);
    }
    return std::any_cast<T>(args[idx]);
}

class Container {
    MapId2Lambda map;

private:
    std::shared_ptr<Container> p_instance;

public:
    void init() {
        p_instance = std::make_shared<Container>();

        Function lambdaRegister = [&](Args args) {
            auto id = get_required_arg<std::string>(args, 0, "invalid id, can't register");
            auto func = get_required_arg<Function>(args, 1, "invalid lambda, can't register");
            std::shared_ptr<RegisterCommand> pCommand = std::make_shared<RegisterCommand>(p_instance, func, id);
            return std::any(pCommand);
        };

        p_instance->map.emplace("IoC.Register", lambdaRegister);
    }


public:

    template<typename T>
    T resolve(const std::string &s, Args &args) {
        auto it = p_instance->map.find(s);
        if (it != p_instance->map.end()) {
            try {
                return std::any_cast<T>(it->second(args));
            } catch (std::bad_any_cast &e) {
                throw std::invalid_argument(
                        std::string("can't resolve: can't cast lambda call result to required type for id: " + s));
            }
        }
        args.insert(args.begin(), s); //not effective, think about optimization
        return resolve<T>("IoC.Register", args);

    };
public:
    class RegisterCommand : public Command::Command {
    private:
        std::shared_ptr<Container> container;
        Function func_to_register;
        std::string id;
    public:
        RegisterCommand(const shared_ptr<Container> &container, const Function &funcToRegister, const std::string &id)
                : container(container), func_to_register(funcToRegister), id(id) {}

    public:
        void execute() override {
            container->map.emplace(id, func_to_register);
        }
    };
};

#endif //OTUS_IOC_H
