#include "socks.h"
#include <stdio.h>

#define BUFFER_SIZE 8

int handle_connection(Sock * const conn);

int main(int argc, char ** argv)
{

    if(5 != argc)
    {
        printf("Usage: %s {IP} {Port} {ipv4|ipv6} {tcp|udp}\n", argv[0]);
        return 1;
    }

    char * ip       = argv[1];
    char * port     = argv[2];
    char * ipver    = argv[3];
    char * protocol = argv[4];
    Sock local;

    if(0 != fill_sock(&local, ip, port, ipver, protocol))
    {
        printf("fill_sock failed\n");
        return 1;
    }

    printf("fill_sock success\n");

    if(0 != bind_sock(&local, _BIND))
    {
        printf("bind_sock _BIND mode failed\n");
        return 1;
    }

    if(TCP == local.socktype)
    {
        // tcp routine
        Sock newconn;

        if(0 != listening_sock(&local))
        {
            printf("listening_sock failed\n");
            return 1;
        }

        if(0 != accept_socks(&local, &newconn))
        {
            printf("accept_socks failed\n");
            return 1;
        }

        handle_connection(&newconn);
    }
    else
    {
        // udp 
        handle_connection(&local);
    }

    wash_sock(&local);

    printf("Looks like that went well\n");
    return 0;
}


int handle_connection(Sock * const conn)
{
    char buffer[BUFFER_SIZE];
    char QUIT[] = "QUIT";
    ssize_t bytes;

    while(1)
    {
        if(0 >= (bytes = sock_recv(conn, buffer, BUFFER_SIZE)))
        {
            if(0 == bytes)
                printf("connection closed\n");
            else
                printf("connection failed\n");
            return 1;
        }

        printf("Received %zd bytes: %s\n", bytes, buffer);

        if(0 == strcmp(buffer, QUIT))
            break;
    }

    printf("Done with connection\n");
    return 0;
}


