/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** portHandler.hpp
*/

#ifndef PORTHANDLER_HPP
#define PORTHANDLER_HPP

#include "ACommandHandler.hpp"

class portHandler: public ACommandHandler {
    public:
        explicit portHandler() = default;
        void handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const override;

    private:
        static bool parsePortArgument(const std::string &arg, std::string &ipAddress, int &port) ;
};

#endif //PORTHANDLER_HPP
