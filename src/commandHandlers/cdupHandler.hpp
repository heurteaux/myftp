/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** cdupHandler.hpp
*/

#ifndef CDUPHANDLER_HPP
#define CDUPHANDLER_HPP

#include "ACommandHandler.hpp"

class cdupHandler: public ACommandHandler {
    public:
    explicit cdupHandler() = default;
    void handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const override;
};

#endif //CDUPHANDLER_HPP
