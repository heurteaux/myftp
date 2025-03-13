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
        enum class TransferMode {
            PASSIVE,
            ACTIVE,
            NONE
        };

        explicit SessionState(const int socketFd, std::string rootDir) : _socketFd(socketFd),
                                                                         _isAuthenticated(false),
                                                                         _rootDirectory(std::move(rootDir)),
                                                                         _currentDirectory("/"),
                                                                         _transferMode(TransferMode::NONE),
                                                                         _dataSocket(-1)
        {
        }

        ~SessionState() = default;

        // getters
        [[nodiscard]] int getSocketFd() const { return _socketFd; }
        [[nodiscard]] bool isAuthenticated() const { return _isAuthenticated; }
        [[nodiscard]] std::string getCurrentDirectory() const { return _currentDirectory; }
        [[nodiscard]] std::string getUsername() const { return _username; }
        [[nodiscard]] std::string getRootDirectory() const { return _rootDirectory; }
        [[nodiscard]] int getDataSocket() const { return _dataSocket; }
        [[nodiscard]] TransferMode getTransferMode() const { return _transferMode; }
        [[nodiscard]] std::string getDataIpAddress() const { return _dataIpAddress; }
        [[nodiscard]] int getDataPort() const { return _dataPort; }

        // setters
        void setAuthStatus(const bool isAuthenticated) { _isAuthenticated = isAuthenticated; }
        void setCurrentDirectory(const std::string &currentDirectory) { _currentDirectory = currentDirectory; }
        void setUsername(const std::string &username) { _username = username; }
        void setDataSocket(const int dataSocketFd) { _dataSocket = dataSocketFd; }
        void setTransferMode(const TransferMode mode) { _transferMode = mode; }
        void setDataIpAddress(const std::string &ipAddress) { _dataIpAddress = ipAddress; }
        void setDataPort(const int port) { _dataPort = port; }

    private:
        int _socketFd;
        bool _isAuthenticated;
        std::string _rootDirectory;
        std::string _currentDirectory;
        std::string _username;
        TransferMode _transferMode;
        int _dataSocket;
        std::string _dataIpAddress;
        int _dataPort;
};

#endif //SESSIONSTATE_HPP
