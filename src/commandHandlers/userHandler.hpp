/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** userHandler.hpp
*/

#ifndef USERHANDLER_HPP
#define USERHANDLER_HPP

#include "ACommandHandler.hpp"

#define GUEST_USER "Anonymous"

class userHandler: public ACommandHandler {
    public:
        explicit userHandler() = default;
        void handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const override;
};

#endif //USERHANDLER_HPP
