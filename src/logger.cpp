/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** logger.cpp
*/

#include "logger.hpp"
#include <iostream>

Logger::Logger(LogLevel level) : _usrLevel(level)
{}

void Logger::log(const LogLevel level, const std::string &msg) const
{
    if (level < getLevel())
        return;
    if (level < LogLevel::WARN) {
        std::cout << std::chrono::system_clock::now();
        std::cout << " [" << logLevelAsStr(level) << "]: " << msg << std::endl;
    } else {
        std::cerr << std::chrono::system_clock::now();
        std::cerr << " [" << logLevelAsStr(level) << "]: " << msg << std::endl;
    }
}

std::string Logger::logLevelAsStr(LogLevel level)
{
    switch (level) {
        case LogLevel::TRACE: return "TRACE";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default:             return "UNKNOWN";
    }
}
