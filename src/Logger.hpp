/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** logger.hpp
*/

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <chrono>

class Logger {
    public:
        enum class LogLevel {
            TRACE,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL
        };

        explicit Logger(LogLevel level);
        ~Logger() = default;
        void log(LogLevel level, const std::string &msg) const;

        LogLevel getLevel() const {return _usrLevel;}
        void setLevel(const LogLevel level) {_usrLevel = level;}

    private:
        LogLevel _usrLevel;
        static std::string logLevelAsStr(LogLevel level);
};



#endif //LOGGER_HPP
