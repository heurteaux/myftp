/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** commandRegister.cpp
*/

#include "commandRegistry.hpp"

void CommandRegistry::registerCommand(const std::string &command, const std::shared_ptr<ICommandHandler> &handler) {
    _commands[command] = handler;
}

std::shared_ptr<ICommandHandler> CommandRegistry::getCommandHandler(const std::string &command) {
    return _commands[command];
}

bool CommandRegistry::contains(const std::string &command) const
{
    return _commands.contains(command);
}

