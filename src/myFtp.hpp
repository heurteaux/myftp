/*
** EPITECH PROJECT, 2025
** cpptemplate
** File description:
** myftp.hpp
*/

#ifndef MYFTP_HPP
#define MYFTP_HPP

#include <exception>
#include <iostream>
#include "networkServer.hpp"
#include "logger.hpp"
#include "protocolResponse.hpp"

#define HELP_FLAG_SHORT "-h"
#define HELP_FLAG_LONG "--help"
#define USAGE "USAGE: ./myftp port path"
#define HELP "\tport is the port number on which the server socket listens\n\tpath is the path to the home directory for the Anonymous user"

namespace ftp
{
    class myFtp {
        public:
            explicit myFtp(int argc, const char **argv);

            void run();

            static void sendResponse(FtpResponse response, int socketFd) ;

            static void sendCustomResponse(const std::string &response, int socketFd) ;

            static std::vector<std::string> tokenize(const std::string &input);

            ~myFtp() = default;

            class invalidArgument final : public std::exception {
                public:
                    explicit invalidArgument(const std::string &message) : _message(message) {}
                    const char *what() const noexcept override { return _message.c_str(); }
                private:
                    std::string _message;
            };
        private:
            int getPort(int argc, const char **argv);

            static std::string getRootDirectory(std::string &path);

            static void fillCommandRegistry();
            static void requestHandler(const std::string &request, std::shared_ptr<SessionState> &session);

            int _port;
            std::string _path;
            Logger _logger;
            NetworkServer _server;
    };
}

#endif //MYFTP_HPP
