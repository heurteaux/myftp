/*
** EPITECH PROJECT, 2025
** cpptemplate
** File description:
** myftp.cpp
*/

#include "myFtp.hpp"
#include "userHandler.hpp"
#include "passHandler.hpp"
#include "cdupHandler.hpp"
#include "commandRegistry.hpp"
#include "cwdHandler.hpp"
#include "quitHandler.hpp"
#include "deleHandler.hpp"
#include "pwdHandler.hpp"
#include "noopHandler.hpp"
#include <filesystem>
#include "pasvHandler.hpp"
#include "portHandler.hpp"
#include "listHandler.hpp"

namespace ftp
{
    int myFtp::getPort(const int argc, const char **argv)
    {
        if (argc == 2 && (std::string(argv[1]) == HELP_FLAG_SHORT || std::string(argv[1]) == HELP_FLAG_LONG)) {
            std::cout << USAGE << std::endl;
            std::cout << HELP << std::endl;
            throw invalidArgument("invalid arguments");
        }
        if (argc != 3) {
            std::cout << USAGE << std::endl;
            throw invalidArgument("invalid arguments");
        }
        _path = argv[2];
        return std::stoi(argv[1]);
    }

    std::string myFtp::getRootDirectory(std::string &path)
    {
        namespace fs = std::filesystem;

        fs::path pathObj(path);
        fs::path absolutePath;

        if (pathObj.is_absolute()) {
            absolutePath = pathObj;
        } else {
            absolutePath = fs::absolute(pathObj);
        }

        if (!fs::exists(absolutePath)) {
            throw invalidArgument("The specified directory does not exist");
        }

        if (!fs::is_directory(absolutePath)) {
            throw invalidArgument("The specified path is not a directory");
        }

        absolutePath = fs::canonical(absolutePath);

        return absolutePath.string();
    }

    myFtp::myFtp(const int argc, const char **argv) : _port(getPort(argc, argv)),
                                                      _path(getRootDirectory(_path)),
                                                      _logger(Logger(Logger::LogLevel::TRACE)),
                                                      _server(NetworkServer(_port, _logger, _path, &requestHandler))
    {}

    void myFtp::fillCommandRegistry()
    {
        auto &registry = CommandRegistry::getInstance();

        registry.registerCommand("USER", std::make_shared<userHandler>());
        registry.registerCommand("PASS", std::make_shared<passHandler>());
        registry.registerCommand("CWD", std::make_shared<cwdHandler>());
        registry.registerCommand("QUIT", std::make_shared<quitHandler>());
        registry.registerCommand("DELE", std::make_shared<deleHandler>());
        registry.registerCommand("PWD", std::make_shared<pwdHandler>());
        registry.registerCommand("NOOP", std::make_shared<noopHandler>());
        registry.registerCommand("CDUP", std::make_shared<cdupHandler>());
        registry.registerCommand("PASV", std::make_shared<pasvHandler>());
        registry.registerCommand("PORT", std::make_shared<portHandler>());
        registry.registerCommand("LIST", std::make_shared<listHandler>());
    }

    void myFtp::run()
    {
        fillCommandRegistry();
        _server.run();
    }

    void myFtp::sendResponse(FtpResponse response, const int socketFd)
    {
        if (!protocolResponses.contains(static_cast<int>(response))) {
            throw std::runtime_error("provided response code is invalid");
        }

        const std::string res = protocolResponses.at(static_cast<int>(response)).getResponse();
        if (write(socketFd, res.c_str(), res.size()) == -1) {
            throw std::runtime_error("an error occurred when writing to network socket");
        }
    }

    void myFtp::sendCustomResponse(const std::string &response, const int socketFd)
    {
        if (write(socketFd, response.c_str(), response.size()) == -1) {
            throw std::runtime_error("an error occurred when writing to network socket");
        }
    }

    std::vector<std::string> myFtp::tokenize(const std::string &input)
    {
        std::vector<std::string> tokens;
        std::stringstream check1(input);
        std::string intermediate;
        while (getline(check1, intermediate, ' ')) {
            tokens.push_back(intermediate);
        }
        return tokens;
    }

    std::string trim(const std::string &str)
    {
        static const char* ws = " \t\n\r\f\v";
        size_t start = str.find_first_not_of(ws);
        if (start == std::string::npos)
            return "";
        size_t end = str.find_last_not_of(ws);
        return str.substr(start, end - start + 1);
    }

    void myFtp::requestHandler(const std::string &request, std::shared_ptr<SessionState> &session)
    {
        const std::string cleanedRequest = trim(request);
        std::vector<std::string> chunkedRequest = tokenize(cleanedRequest);

        if (chunkedRequest.empty()) {
            sendResponse(FtpResponse::SYNTAX_ERROR, session->getSocketFd());
            return;
        }
        const std::vector args(chunkedRequest.begin() + 1, chunkedRequest.end());
        std::string command = chunkedRequest[0];

        for (auto &c: command) {
            c = static_cast<char>(std::toupper(c));
        }

        if (CommandRegistry::getInstance().contains(command)) {
            CommandRegistry::getInstance().getCommandHandler(command)->handleRequest(args, session);
        } else {
            sendResponse(FtpResponse::SYNTAX_ERROR, session->getSocketFd());
        }
    }
}
