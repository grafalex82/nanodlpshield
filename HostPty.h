#pragma once

#include <string>
#include <memory>

class HostPty
{
    int _master;
    int _slave;


    static const int BUF_SIZE = 4096;

    char _buf[BUF_SIZE];
    size_t _buf_ptr;
    size_t _contents_size;

public:
    HostPty(const std::string & pty_name);
    ~HostPty();

    std::string nextString();
};
