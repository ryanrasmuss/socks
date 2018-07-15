#include "socks.h"
#include <stdio.h>

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

    if(0 != bind_sock(&local, _CONN))
    {
        printf("bind_sock _CONN mode failed\n");
        return 1;
    }

    char hello[] = "hello";
    char quit[] = "QUIT";

    if(0 >= sock_send(&local, hello, sizeof(hello)))
    {
        printf("Problem sending\n");
        wash_sock(&local);
        return 1;
    }

    if(0 >= sock_send(&local, quit, sizeof(quit)))
    {
        printf("Problem sending quit\n");
        wash_sock(&local);
        return 1;
    }

    wash_sock(&local);

    printf("Looks like that went well\n");
    return 0;
}
