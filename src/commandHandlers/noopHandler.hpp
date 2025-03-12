/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** noopHandler.hpp
*/

#ifndef NOOPHANDLER_HPP
#define NOOPHANDLER_HPP

#include "ACommandHandler.hpp"

class noopHandler: public ACommandHandler {
    public:
    explicit noopHandler() = default;
    void handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const override;
};

#endif //NOOPHANDLER_HPP
