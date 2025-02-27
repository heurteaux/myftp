# MyFTP - File Transfer Protocol Server

A custom implementation of an FTP server in C/C++ according to RFC959 specifications.

## Overview

This project implements a File Transfer Protocol (FTP) server that can handle multiple client connections simultaneously. The server follows the RFC959 protocol specifications and supports both active and passive data transfer modes.

## Assignment
[View project assignment](assignment.pdf)
## Building

### Setup CMake
```bash
cmake -S . -B build
```
### Build
```bash
# Release build
cmake --build build

# Build unit tests
cmake --build build --target unit_tests

# Debug build
cmake --build build --target dbg_build
```

## Usage

```bash
./myftp port path
```

- `port`: The port number on which the server socket listens
- `path`: The path to the home directory for the Anonymous user

## Implementation Details

- **Authentication**: Supports Anonymous login with empty password
- **Connection Management**: Handles multiple clients simultaneously using poll
- **Data Transfer**: Uses fork for data transfer operations
- **Transfer Modes**: Supports both active and passive modes

## FTP Commands

The server implements the basic FTP commands as defined in RFC959:

- `USER`: Specify user for authentication
- `PASS`: Specify password for authentication
- `CWD`: Change working directory
- `CDUP`: Change to parent directory
- `QUIT`: Terminate the connection
- `DELE`: Delete a file
- `PWD`: Print working directory
- `PASV`: Enter passive mode
- `PORT`: Enter active mode
- `HELP`: List available commands
- `NOOP`: No operation
- `RETR`: Retrieve a file
- `STOR`: Store a file
- `LIST`: List files in the current directory