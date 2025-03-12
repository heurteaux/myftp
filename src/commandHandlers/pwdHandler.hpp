/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** pwdHandler.hpp
*/

#ifndef PWDHANDLER_HPP
#define PWDHANDLER_HPP

#include "ACommandHandler.hpp"

class pwdHandler: public ACommandHandler {
    public:
    explicit pwdHandler() = default;
    void handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const override;
};

#endif //PWDHANDLER_HPP
