#include "HostPty.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pty.h>

using namespace std;

HostPty::HostPty(const string & pty_name)
    : _pty_name(pty_name)
    , _bufPos(0)
{
    char name[100] = {0};
    int res = openpty(&_master, &_slave, name, NULL, NULL);
    printf("Openpty returned %d\n", res);
    printf("Name: %s\n", name);

    if(res != 0)
        throw std::runtime_error("Cannot open HostPty");

    unlink(pty_name.c_str());
    res = symlink(name, pty_name.c_str());
    if(res < 0)
        throw std::runtime_error("Cannot create pty symlink");
}

HostPty::~HostPty()
{
    unlink(_pty_name.c_str());
    close(_master);
    close(_slave);
}

bool HostPty::receiveNextString(string & outStr)
{
    // Now receive next portion of data
    struct timeval tv{0, 1000};

    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(_master, &rfds);

    if (select(_master + 1, &rfds, NULL, NULL, &tv))
    {
        // Read symbols one by one until new line is found
        size_t received_size = read(_master, _buf + _bufPos, 1);

        // Stopping at new line
        if(_buf[_bufPos] == '\n')
        {
            outStr = string(_buf, _bufPos);
            _bufPos = 0;
            return true;
        }

        _bufPos++;
    }

    return false;
}
