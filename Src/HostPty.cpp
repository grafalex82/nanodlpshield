#include "HostPty.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pty.h>

using namespace std;

HostPty::HostPty(const string & pty_name)
    : _pty_name(pty_name)
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

string HostPty::nextString()
{
    static const int BUF_SIZE = 4096;
    char buf[BUF_SIZE];
    size_t idx = 0;

    while(true)
    {
        // Now receive next portion of data
        fd_set rfds;
        struct timeval tv{0, 0};
        FD_ZERO(&rfds);
        FD_SET(_master, &rfds);

        if (select(_master + 1, &rfds, NULL, NULL, &tv))
        {
            // Read symbols one by one until new line is found
            size_t received_size = read(_master, buf + idx, 1);

            // Stopping at new line
            if(buf[idx] == '\n')
                break;

            idx++;
        }
    }

    return string(buf, idx);
}
