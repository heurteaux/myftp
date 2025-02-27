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

#define HELP_FLAG_SHORT "-h"
#define HELP_FLAG_LONG "--help"
#define USAGE "USAGE: ./myftp port path"
#define HELP "\tport is the port number on which the server socket listens\n\tpath is the path to the home directory for the Anonymous user"

namespace ftp
{
    class myftp {
        public:
            explicit myftp() = default;
            ~myftp() = default;

            static void run(int argc, const char **argv);

            class invalidArgument final : public std::exception {
                public:
                    explicit invalidArgument(const std::string &message) : _message(message) {}
                    const char *what() const noexcept override { return _message.c_str(); }
                private:
                    std::string _message;
            };
    };
}

#endif //MYFTP_HPP
