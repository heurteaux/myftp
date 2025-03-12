/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** deleHandler.hpp
*/

#ifndef DELEHANDLER_HPP
#define DELEHANDLER_HPP

#include "ACommandHandler.hpp"

class deleHandler: public ACommandHandler {
    public:
    explicit deleHandler() = default;
    void handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const override;
};

#endif //DELEHANDLER_HPP
