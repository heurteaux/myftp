/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** sessionState.hpp
*/

#ifndef SESSIONSTATE_HPP
#define SESSIONSTATE_HPP

#include <iostream>
#include <utility>

class SessionState {
    public:
        explicit SessionState(const int socketFd, std::string rootDir) : _socketFd(socketFd),
                                                                         _isAuthenticated(false),
                                                                         _rootDirectory(std::move(rootDir)),
                                                                         _currentDirectory("/")
        {
        }

        ~SessionState() = default;

        // getters
        [[nodiscard]] int getSocketFd() const { return _socketFd; }
        [[nodiscard]] bool isAuthenticated() const { return _isAuthenticated; }
        [[nodiscard]] std::string getCurrentDirectory() const { return _currentDirectory; }
        [[nodiscard]] std::string getUsername() const { return _username; }
        [[nodiscard]] std::string getRootDirectory() const { return _rootDirectory; }

        // setters
        void setAuthStatus(bool isAuthenticated) { _isAuthenticated = isAuthenticated; }
        void setCurrentDirectory(const std::string &currentDirectory) { _currentDirectory = currentDirectory; }
        void setUsername(const std::string &username) { _username = username; }

    private:
        int _socketFd;
        bool _isAuthenticated;
        std::string _rootDirectory;
        std::string _currentDirectory;
        std::string _username;
};

#endif //SESSIONSTATE_HPP
