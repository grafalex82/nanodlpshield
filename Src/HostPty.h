#pragma once

#include <string>
#include <memory>

class HostPty
{
    int _master;
    int _slave;
    std::string _pty_name;

    static const int BUF_SIZE = 4096;
    char _buf[BUF_SIZE];
    size_t _bufPos;

public:
    HostPty(const std::string & pty_name);
    ~HostPty();

    bool receiveNextString(std::string & outStr);
    void write(const std::string & str);
};
