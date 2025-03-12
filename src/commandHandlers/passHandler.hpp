/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** passHandler.hpp
*/

#ifndef PASSHANDLER_HPP
#define PASSHANDLER_HPP

#include "ACommandHandler.hpp"

class passHandler: public ACommandHandler {
    public:
        explicit passHandler() = default;
        void handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const override;
};

#endif //PASSHANDLER_HPP
