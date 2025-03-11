/*
** EPITECH PROJECT, 2025
** cpptemplate
** File description:
** myftp.cpp
*/

#include "myFtp.hpp"

namespace ftp
{
    int myFtp::getPort(const int argc, const char **argv)
    {
        if (argc == 2 && (std::string(argv[1]) == HELP_FLAG_SHORT || std::string(argv[1]) == HELP_FLAG_LONG)) {
            std::cout << USAGE << std::endl;
            std::cout << HELP << std::endl;
            throw invalidArgument("invalid arguments");
        }
        if (argc != 3) {
            std::cout << USAGE << std::endl;
            throw invalidArgument("invalid arguments");
        }
        _path = argv[2];
        return std::stoi(argv[1]);
    }

    myFtp::myFtp(const int argc, const char **argv) : _port(getPort(argc, argv)),
                                                      _logger(Logger(Logger::LogLevel::TRACE)),
                                                      _server(NetworkServer(_port, _logger, _path))
    {}

    void myFtp::run()
    {
        _server.run();
    }
}
