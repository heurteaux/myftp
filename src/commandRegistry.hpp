/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** commandRegister.hpp
*/

#ifndef COMMANDREGISTER_HPP
#define COMMANDREGISTER_HPP

#include <unordered_map>
#include <vector>
#include <functional>
#include <iostream>
#include <memory>

#include "ACommandHandler.hpp"

class CommandRegistry {
    public:
        static CommandRegistry &getInstance() {
            static CommandRegistry instance;
            return instance;
        }

        CommandRegistry(const CommandRegistry&) = delete;
        CommandRegistry &operator=(const CommandRegistry&) = delete;

        void registerCommand(const std::string&, const std::shared_ptr<ICommandHandler>&);
        std::shared_ptr<ICommandHandler> getCommandHandler(const std::string &command);
        bool contains(const std::string &command) const;

    private:
        CommandRegistry() = default;
        ~CommandRegistry() = default;
        std::unordered_map<std::string, std::shared_ptr<ICommandHandler>> _commands {};
};

#endif //COMMANDREGISTER_HPP
