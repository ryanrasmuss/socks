#include <stdio.h>
#include "socks.c"
#include <stdio.h>

int main(int argc, char ** argv)
{
    if(argc != 3)
    {
        printf("Usage: %s <IP ADDRESS> <PORT NUMBER>\n", argv[0]);
        return 1;
    }

    Sock local;

    if(0 != set_serverinfo(&local, argv[1], argv[2]))
    {
        printf("Failed\n");
        return 1;
    }

    printf("set_serverinfo success\n");

    if(-1 == make_socket(&local))
    {
        printf("Making socket failed\n");
        return 1;
    }

    printf("make_socket success\n");

    if(0 != bind_socket(&local))
    {
        printf("Binding socket failed\n");
        return 1;
    }

    printf("bind_socket success\n");

    if(0 != listening_sock(&local))
    {
        printf("Listening sock failed\n");
        return 1;
    }

    printf("listening_sock success\n");

    Peer_Sock new_conn;

    if(0 != accept_socks(&local, &new_conn))
    {
        printf("accepting new sock failed\n");
        return 1;
    }

    printf("accept_sock success\n");

    wash_sock(&local);

    printf("free_all success\n");

    printf("Looks like everything went well.\n");

    return 0;
}
