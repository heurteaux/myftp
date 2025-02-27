/*
** EPITECH PROJECT, 2025
** cpptemplate
** File description:
** myftp.cpp
*/

#include "myftp.hpp"

namespace ftp
{
    void myftp::run(const int argc, const char **argv)
    {
        if (argc == 2 && (std::string(argv[1]) == HELP_FLAG_SHORT || std::string(argv[1]) == HELP_FLAG_LONG)) {
            std::cout << USAGE << std::endl;
            std::cout << HELP << std::endl;
            return;
        }
        if (argc != 3) {
            throw std::invalid_argument(USAGE);
        }
        std::cout << "Valid arguments" << std::endl;
    }
}
