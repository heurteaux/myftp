/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** quitHandler.hpp
*/

#ifndef QUITHANDLER_HPP
#define QUITHANDLER_HPP

#include "ACommandHandler.hpp"

class quitHandler: public ACommandHandler {
    public:
    explicit quitHandler() = default;
    void handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const override;
};


#endif //QUITHANDLER_HPP
