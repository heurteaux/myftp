/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** ICommandHandler.hpp
*/

#ifndef ICOMMANDHANDLER_HPP
#define ICOMMANDHANDLER_HPP

#include <iostream>
#include <memory>
#include <vector>
#include "sessionState.hpp"

class ICommandHandler {
    public:
        virtual ~ICommandHandler() = default;

        virtual void handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &session) const = 0;
};

#endif //ICOMMANDHANDLER_HPP
