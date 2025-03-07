/*
** EPITECH PROJECT, 2025
** cpptemplate
** File description:
** myftp.cpp
*/

#include "MyFtp.hpp"

#include "NetworkServer.hpp"

namespace ftp
{
    void MyFtp::run(const int argc, const char **argv)
    {
        if (argc == 2 && (std::string(argv[1]) == HELP_FLAG_SHORT || std::string(argv[1]) == HELP_FLAG_LONG)) {
            std::cout << USAGE << std::endl;
            std::cout << HELP << std::endl;
            return;
        }
        if (argc != 3) {
            throw std::invalid_argument(USAGE);
        }

        network::NetworkServer netServer(8080);
        netServer.handleConnections();
    }
}
