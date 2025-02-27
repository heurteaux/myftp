/*
** EPITECH PROJECT, 2025
** nanotekspice
** File description:
** main.cpp
*/

#include <exception>
#include <iostream>
#include "myftp.hpp"

int main(const int argc, const char **argv)
{
    try {
        ftp::myftp::run(argc, argv);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
