/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** protocolResponse.hpp
*/

#ifndef PROTOCOLRESPONSE_HPP
#define PROTOCOLRESPONSE_HPP

#include <iostream>
#include <unordered_map>
#include <utility>

class ProtocolResponse {
    public:
        explicit ProtocolResponse(const int code, std::string msg) : code(code), msg(std::move(msg)) {}
        ~ProtocolResponse() = default;

        [[nodiscard]] std::string getResponse() const;
        int code;
        std::string msg;
};

enum class FtpResponse {
    SYNTAX_ERROR = 500,
    SYNTAX_ERROR_PARAMS = 501,
    CMD_NOT_IMPLEMENTED = 502,
    BAD_CMD_SEQUENCE = 503,
    CMD_PARAM_NOT_IMPLEMENTED = 504,

    SERVICE_UNAVAILABLE = 421,
    CANT_OPEN_DATA_CONN = 425,
    CONN_CLOSED_TRANSFER_ABORTED = 426,
    FILE_UNAVAILABLE = 450,
    ACTION_ABORTED_LOCAL_ERROR = 451,
    INSUFFICIENT_STORAGE = 452,

    NOT_LOGGED_IN = 530,
    NEED_ACCOUNT_FOR_STORING = 532,
    FILE_UNAVAILABLE_NOT_FOUND = 550,
    PAGE_TYPE_UNKNOWN = 551,
    EXCEEDED_STORAGE_ALLOCATION = 552,
    FILENAME_NOT_ALLOWED = 553,

    COMMAND_OK = 200,
    COMMAND_SUPERFLUOUS = 202,
    SYSTEM_STATUS = 211,
    DIRECTORY_STATUS = 212,
    FILE_STATUS = 213,
    HELP_MESSAGE = 214,
    SYSTEM_TYPE = 215,

    RESTART_MARKER = 110,
    SERVICE_READY_IN_MINUTES = 120,
    DATA_CONN_OPEN_STARTING = 125,
    FILE_STATUS_OK_OPENING_CONN = 150,

    SERVICE_READY = 220,
    SERVICE_CLOSING = 221,
    DATA_CONN_OPEN_NO_TRANSFER = 225,
    CLOSING_DATA_CONN = 226,
    ENTERING_PASSIVE_MODE = 227,
    USER_LOGGED_IN = 230,
    FILE_ACTION_COMPLETED = 250,
    PATHNAME_CREATED = 257,

    USERNAME_OK_NEED_PASSWORD = 331,
    NEED_ACCOUNT_FOR_LOGIN = 332,
    FILE_ACTION_PENDING = 350
};

static std::unordered_map<int, ProtocolResponse> protocolResponses = {
    // 2xx Success codes
    {200, ProtocolResponse(200, "Command okay.")},
    {202, ProtocolResponse(202, "Command not implemented, superfluous at this site.")},
    {211, ProtocolResponse(211, "System status, or system help reply.")},
    {212, ProtocolResponse(212, "Directory status.")},
    {213, ProtocolResponse(213, "File status.")},
    {214, ProtocolResponse(214, "Help message.")},
    {215, ProtocolResponse(215, "NAME system type.")},
    {220, ProtocolResponse(220, "Service ready for new user.")},
    {221, ProtocolResponse(221, "Service closing control connection.")},
    {225, ProtocolResponse(225, "Data connection open; no transfer in progress.")},
    {226, ProtocolResponse(226, "Closing data connection.")},
    {227, ProtocolResponse(227, "Entering Passive Mode (h1,h2,h3,h4,p1,p2).")},
    {230, ProtocolResponse(230, "User logged in, proceed.")},
    {250, ProtocolResponse(250, "Requested file action okay, completed.")},
    {257, ProtocolResponse(257, "{} created.")},

    // 1xx Information codes
    {110, ProtocolResponse(110, "Restart marker reply.")},
    {120, ProtocolResponse(120, "Service ready in nnn minutes.")},
    {125, ProtocolResponse(125, "Data connection already open; transfer starting.")},
    {150, ProtocolResponse(150, "File status okay; about to open data connection.")},

    // 3xx Intermediate codes
    {331, ProtocolResponse(331, "User name okay, need password.")},
    {332, ProtocolResponse(332, "Need account for login.")},
    {350, ProtocolResponse(350, "Requested file action pending further information.")},

    // 4xx Temporary Error codes
    {421, ProtocolResponse(421, "Service not available, closing control connection.")},
    {425, ProtocolResponse(425, "Can't open data connection.")},
    {426, ProtocolResponse(426, "Connection closed; transfer aborted.")},
    {450, ProtocolResponse(450, "Requested file action not taken. File unavailable.")},
    {451, ProtocolResponse(451, "Requested action aborted. Local error in processing.")},
    {452, ProtocolResponse(452, "Requested action not taken. Insufficient storage space in system.")},

    // 5xx Permanent Error codes
    {500, ProtocolResponse(500, "Syntax error, command unrecognized.")},
    {501, ProtocolResponse(501, "Syntax error in parameters or arguments.")},
    {502, ProtocolResponse(502, "Command not implemented.")},
    {503, ProtocolResponse(503, "Bad sequence of commands.")},
    {504, ProtocolResponse(504, "Command not implemented for that parameter.")},
    {530, ProtocolResponse(530, "Not logged in.")},
    {532, ProtocolResponse(532, "Need account for storing files.")},
    {550, ProtocolResponse(550, "Requested action not taken. File unavailable.")},
    {551, ProtocolResponse(551, "Requested action aborted. Page type unknown.")},
    {552, ProtocolResponse(552, "Requested file action aborted. Exceeded storage allocation.")},
    {553, ProtocolResponse(553, "Requested action not taken. File name not allowed.")}
};

#endif //PROTOCOLRESPONSE_HPP
