#include "HostPty.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pty.h>

using namespace std;

HostPty::HostPty(const string & HostPty_name)
    : _buf_ptr(0)
    , _contents_size(0)
{
    char name[100] = {0};
    int res = openpty(&_master, &_slave, name, NULL, NULL);
    printf("Openpty returned %d\n", res);
    printf("Name: %s\n", name);

    if(res != 0)
        throw std::runtime_error("Cannot open HostPty");

    // TODO: make a symlink here
}

HostPty::~HostPty()
{
    close(_master);
    close(_slave);
}

string HostPty::nextString()
{
    while(1)
    {
        // Search for the next complete string in a buffer
        for(size_t idx = _buf_ptr; idx <  _contents_size; idx++)
        {
            // Found a string
            if(_buf[idx] == '\n')
            {
                string ret = string(_buf + _buf_ptr, idx - _buf_ptr);
                _buf_ptr = idx + 1;
                return ret;
            }
        }

        // We have not found a complete line. Pack previous buffer content closer to the beginning and try to get another chunk
        memmove(_buf, _buf + _buf_ptr, _contents_size - _buf_ptr);
        _contents_size -= _buf_ptr;
        _buf_ptr = 0;

        // Now receive next portion of data
        fd_set rfds;
        struct timeval tv{0, 0};
        FD_ZERO(&rfds);
        FD_SET(_master, &rfds);

        if (select(_master + 1, &rfds, NULL, NULL, &tv))
        {
            size_t received_size = read(_master, _buf + _contents_size, BUF_SIZE - _contents_size);
            _contents_size += received_size;
        }
    }

    return "";
}
