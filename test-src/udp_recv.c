#include "socks.h"
#include <stdio.h>

int main(int argc, char ** argv)
{
    printf("-------- UDP Test -------\n");

    if(4 != argc)
    {
        printf("Usage: %s {IP ADDRESS} {PORT NUMBER} {IPV4|IPV6}\n", argv[0]);
        return 1;
    }

    Sock local;
    int ret;

    if(0 != (ret = fill_sock(&local, argv[1], argv[2], argv[3], "udp")))
    {
        printf("fill_sock failed w/ %d\n", ret);
        return 1;
    }

    printf("fill_sock success\n");
    
    if(0 != bind_sock(&local, 1))
    {
        fprintf(stderr, "bind_sock bind mode failed\n");
        return 1;
    }

    printf("bind_sock bind mode success\n");

    const size_t buffer = 32;
    char Message[buffer];

    if(-1 == (ret = sock_recv(&local, Message, sizeof(Message))))
    {
        printf("sock_recv failed: %d\n", ret);
        return 1;
    }

    printf("sock_recv success %d\n", ret);

    printf("Here is the Message: %s\n", Message);

    printf("Bye :)\n");

    return 0;
}
