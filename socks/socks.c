#include "socks.h"

// close
#include <unistd.h>

// debugging 
#include <string.h>
#include <errno.h>
#include <stdio.h>
// remove above later

int wash_sock(Sock * const sock)
{
    freeaddrinfo(sock->serverinfo);
    close(sock->socketfd);

    return 0;
}

int set_serverinfo(Sock * const sock, const char * const ip_address, const char * const port)
{
    memset(&(sock->settings), 0, sizeof(sock->settings));

    sock->settings.ai_family   = AF_UNSPEC;
    sock->settings.ai_socktype = SOCK_STREAM;
    //sock->settings.ai_flags    = AI_PASSIVE;
    sock->settings.ai_flags     = AI_CANONNAME;

    int status;

    if(0 != (status = getaddrinfo(ip_address, port, &(sock->settings), &sock->serverinfo)))
    {
        printf("ERROR: getaddrinfo returned %d\n", status);
        printf("ERROR: %s\n", gai_strerror(status));
        return -1;
    }

    return 0;
}


// return a socket fd
// socket is a end point(ep) for communication and 
// returns a file descriptor that refers to that endp point
int make_socket(Sock * const sock)
{
    int returned;

    if(-1 == (returned = socket(sock->serverinfo->ai_family, sock->serverinfo->ai_socktype,
                    sock->serverinfo->ai_protocol)))
    {
        printf("ERROR: socket returned %d\n", returned);
        printf("ERROR: %s\n", strerror(errno));
        wash_sock(sock);
        return -1;
    }

    sock->socketfd = returned;

    return returned;
}

// when a socket is created with socket, it exists in a name space (address family) but
// has no address assigned to it
// bind() assigns the address specified by addr to the socket referred to by the file desc sockfd
// addrlen specifies the size, in bytes, of the address structure pointed to by addr
// traditionally, this process is called "assigning a name to a socket"
int bind_socket(Sock * const sock)
{
    int returned;

    if(0 != (returned = bind(sock->socketfd, sock->serverinfo->ai_addr,
                    sock->serverinfo->ai_addrlen)))
    {
        printf("ERROR: bind returned %d\n", returned);
        printf("ERROR: %s\n", strerror(errno));
        wash_sock(sock);
        return -1;
    }

    return 0;
}

// sockfd must refer to a socket of type SOCK_STREAM (tcp) or SOCK_SEQPACKET
// return 0 for success, -1 for error (errno set)
int listening_sock(Sock * const sock)
{
    int returned;

    if(-1 == (returned = listen(sock->socketfd, QUEUE_SIZE)))
    {
        printf("ERROR: listen returned %d\n", returned);
        printf("ERROR: %s\n", strerror(errno));
        wash_sock(sock);
        return -1;
    }

    return 0;
}

int accept_socks(Sock * const sock, Sock * const new_sock)
{
    if(NULL == sock)
        return -2;

    if(NULL == new_sock)
        return -3;

    new_sock->settings.ai_addrlen = sizeof(*new_sock->settings.ai_addr);

    if(-1 == (new_sock->socketfd = accept(sock->socketfd, new_sock->settings.ai_addr, 
                    &new_sock->settings.ai_addrlen)))
    {
        printf("ERROR: accept returned %d\n", new_sock->socketfd);
        printf("ERROR: %s\n", strerror(errno));
        wash_sock(sock);
        return -1;
    }

    return 0;
}

int connect_socks(Sock * const sock)
{
    int returned;

    if(-1 == (returned = connect(sock->socketfd, sock->serverinfo->ai_addr,
                    sock->serverinfo->ai_addrlen)))
    {
        printf("ERROR: connect returned %d\n", returned);
        printf("ERROR: %s\n", strerror(errno));
        wash_sock(sock);
        return -1;
    }

    return 0;
}

int sock_send(Sock * const sock, const char * const message, const size_t length)
{
    if(NULL == sock)
        return -2;

    if(NULL == message)
        return -3;

    ssize_t returned;

    // XXX flag support later
    if(-1 == (returned = send(sock->socketfd, message, length, 0)))
    {
        printf("ERROR: send returned %zd\n", returned);
        printf("ERROR: %s\n", strerror(errno));
        return -1;
    }
    
    return returned;
}

int sock_recv(Sock * const sock, char * const buffer, const size_t size)
{
    if(NULL == sock)
        return -2;
    
    if(NULL == buffer)
        return -3;

    ssize_t returned;

    // XXX flag support later
    if(-1 == (returned = recv(sock->socketfd, buffer, size, 0)))
    {
        printf("ERROR: recv returned %zd\n", returned);
        printf("ERROR: %s\n", strerror(errno));
        return -1;
    }

    return returned;
}
