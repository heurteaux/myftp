/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** listHandler.hpp
*/

#ifndef LISTHANDLER_HPP
#define LISTHANDLER_HPP

#include "ACommandHandler.hpp"

class listHandler: public ACommandHandler {
    public:
        explicit listHandler() = default;
        void handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const override;

    private:
        void sendDirectoryListing(const std::string &path, int dataSocket) const;
        std::string getFormattedFileInfo(const std::string &path, const std::string &filename) const;

        static void executeListCommand(const std::string &path, int dataSocket) ;

        static void handleActiveMode(const std::string &path, std::shared_ptr<SessionState> &state) ;
        static void handlePassiveMode(const std::string &path, std::shared_ptr<SessionState> &state) ;
};

#endif //LISTHANDLER_HPP
