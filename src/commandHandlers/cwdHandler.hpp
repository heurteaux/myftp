/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** cwdHandler.hpp
*/

#ifndef CWDHANDLER_HPP
#define CWDHANDLER_HPP

#include "ACommandHandler.hpp"

class cwdHandler: public ACommandHandler {
    public:
    explicit cwdHandler() = default;
    void handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const override;
};


#endif //CWDHANDLER_HPP
