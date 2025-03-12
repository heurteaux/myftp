/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** aCommandHandler.hpp
*/

#ifndef ACOMMANDHANDLER_HPP
#define ACOMMANDHANDLER_HPP

#include "ICommandHandler.hpp"
#include "protocolResponse.hpp"
#include <unistd.h>

class ACommandHandler : public ICommandHandler {
    protected:
        static void sendResponse(FtpResponse response, const std::shared_ptr<SessionState> &session) ;
        static void sendCustomResponse(const std::string &response, const std::shared_ptr<SessionState> &session) ;
};

#endif //ACOMMANDHANDLER_HPP
