/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** retrHandler.hpp
*/

#ifndef RETRHANDLER_HPP
#define RETRHANDLER_HPP

#include "ACommandHandler.hpp"

class retrHandler: public ACommandHandler {
    public:
        explicit retrHandler() = default;
        void handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const override;

    private:
        static void handleActiveMode(const std::string &filePath, std::shared_ptr<SessionState> &state) ;
        static void handlePassiveMode(const std::string &filePath, std::shared_ptr<SessionState> &state) ;
        static void sendFile(const std::string &filePath, int dataSocket) ;
};

#endif //RETRHANDLER_HPP
