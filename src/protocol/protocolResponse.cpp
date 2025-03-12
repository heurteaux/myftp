/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** protocolResponse.cpp
*/

#include "protocolResponse.hpp"

std::string ProtocolResponse::getResponse() const
{
    return std::to_string(code) + " " + msg + "\r\n";
}