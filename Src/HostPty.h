#pragma once

#include <string>
#include <memory>

class HostPty
{
    int _master;
    int _slave;
    std::string _pty_name;

public:
    HostPty(const std::string & pty_name);
    ~HostPty();

    std::string nextString();
};
