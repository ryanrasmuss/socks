#include "socks.h"
#include <stdio.h>
#include <fcntl.h>

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
        printf("fill_sock failed w/ %d\n", ret);
        return 1;
    }
    
    printf("fill_sock success\n");

    if(0 != bind_sock(&local, 0))
    {
        fprintf(stderr, "bind_sock connect mode failed\n");
        return 1;
    }

    printf("bind_sock connect mode success\n");

    char hello[] = "HELLO :)";
    int returned;

    if(0 > (returned = sock_send(&local, hello, sizeof(hello))))
    {
        printf("sock_send FAILED\n");
        wash_sock(&local);
        return 1;
    }

    printf("sock_send success\n");
    printf("sent %lu bytes: %s\n", sizeof(hello), hello);

    size_t buffer_size = 32;
    char buffer[buffer_size];

    if(0 > (returned = sock_recv(&local, buffer, buffer_size)))
    {
        printf("sock_recv FAILED\n");
        wash_sock(&local);
        return 1;
    }

    printf("sock_recv success\n");
    printf("Got %d bytes: %s\n", returned, buffer);

    wash_sock(&local);

    printf("wash_sock success\n");

    printf("Looks like everything works\n");

    return 0;
}
