/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** pasvHandler.hpp
*/

#ifndef PASVHANDLER_HPP
#define PASVHANDLER_HPP

#include "ACommandHandler.hpp"

class pasvHandler: public ACommandHandler {
    public:
        explicit pasvHandler() = default;
        void handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const override;
    private:
        static int createPassiveSocket(std::string &ipAddress, int &port) ;
};

#endif //PASVHANDLER_HPP
