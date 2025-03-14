/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** helpHandler.hpp
*/

#ifndef HELPHANDLER_HPP
#define HELPHANDLER_HPP

#include "ACommandHandler.hpp"

class helpHandler: public ACommandHandler {
    public:
    explicit helpHandler() = default;
    void handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const override;
};

#endif //HELPHANDLER_HPP
