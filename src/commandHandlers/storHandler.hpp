/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** storHandler.hpp
*/

#ifndef STORHANDLER_HPP
#define STORHANDLER_HPP

#include "ACommandHandler.hpp"

class storHandler final : public ACommandHandler {
    public:
        explicit storHandler() = default;
        void handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const override;

    private:
        void handleActiveMode(const std::string &filePath, std::shared_ptr<SessionState> &state) const;
        void handlePassiveMode(const std::string &filePath, std::shared_ptr<SessionState> &state) const;
        [[nodiscard]] bool receiveFile(const std::string &filePath, int dataSocket) const;
};

#endif //STORHANDLER_HPP
