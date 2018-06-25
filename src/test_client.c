#include <stdio.h>
#include "socks.c"

#include <fcntl.h>

int main(int argc, char ** argv)
{
    if(3 != argc)
    {
        printf("Usage: ./ <IP ADDRESS> <PORT NUMBER>\n");
        return 1;
    }

    Sock local;

    if(0 != set_serverinfo(&local, argv[1], argv[2]))
    {
        printf("set_serverinfo FAILED\n");
        return 1;
    }

    printf("set_serverinfo success\n");

    if(-1 == make_socket(&local))
    {
        printf("make_socket failed\n");
        return 1;
    }

    printf("make_socket success\n");

    if(0 != connect_socks(&local))
    {
        printf("connect_socks FAILED\n");
        return 1;
    }

    printf("connect_sock success\n");

    wash_sock(&local);

    printf("Looks like everything works\n");

    return 0;
}
