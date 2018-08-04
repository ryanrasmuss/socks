#include "socks.h"
#include <stdio.h>

int main(int argc, char ** argv)
{
    if(4 != argc)
    {
        printf("Usage: %s {IP ADDRESS} {PORT NUMBER} {IPV4|IPV6}\n", argv[0]);
        return 1;
    }

    Sock local;

    int ret;

    if(0 != (ret = fill_sock(&local, argv[1], argv[2], argv[3], "tcp")))
    {
        fprintf(stderr, "fill_sock failed w/ %d \n", ret);
        return 1;
    }

    printf("fill_sock success\n");

    if(0 != bind_sock(&local, 1))
    {
        printf("bind_sock bind mode\n");
        return 1;
    }
    
    printf("bind_sock bind mode success\n");

    if(0 != listening_sock(&local))
    {
        printf("Listening sock failed\n");
        return 1;
    }

    printf("listening_sock success\n");

    Sock new_conn;

    if(0 != accept_socks(&local, &new_conn))
    {
        printf("accepting new sock failed\n");
        return 1;
    }

    printf("accept_sock success\n");

    size_t buffer_size = 32;
    char buffer[buffer_size];
    int returned;

    if(0 > (returned = sock_recv(&new_conn, buffer, buffer_size)))
    {
        printf("sock_recv FAILED\n");
        wash_sock(&local);
        return 1;
    }

    printf("sock_recv sucess\n");
    printf("Got %d bytes: %s\n", returned, buffer);

    char hi[] = "Hi!";

    if(0 > (returned = sock_send(&new_conn, hi, sizeof(hi))))
    {
        printf("sock_send FAILED\n");
        wash_sock(&local);
        return 1;
    }

    printf("sock_send success\n");
    printf("sent %lu bytes: %s\n", sizeof(hi), hi);

    wash_sock(&local);

    printf("wash_sock success\n");

    printf("Looks like everything went well.\n");

    return 0;
}
