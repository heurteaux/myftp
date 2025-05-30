/*
** EPITECH PROJECT, 2025
** nanotekspice
** File description:
** main.cpp
*/

#include <exception>
#include <iostream>
#include "myFtp.hpp"

int main(const int argc, const char **argv)
{
    try {
        ftp::myFtp ftp(argc, argv);
        ftp.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
