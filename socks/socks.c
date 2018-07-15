#include "socks.h"

int wash_sock(Sock * const sock)
{
    if(NULL == sock) return -1;

    close(sock->socketfd);

    return 0;
}

int fill_sock(Sock * const sock, const char * ip_address, const char * port, 
        const char * const ip_version, const char * const protocol)
{
    if(NULL == sock)
        return -1;
    if(NULL == ip_address)
        return -2;
    if(NULL == port)
        return -3;
    if(NULL == ip_version)
        return -4;
    if(NULL == protocol)
        return -5;

    memset(&(sock->settings), 0, sizeof(sock->settings));

    if(0 == strcmp(ip_version, "IPV4") || 0 == strcmp(ip_version, "ipv4"))
        sock->settings.ai_family = AF_INET;
    else if(0 == strcmp(ip_version, "IPV6") || 0 == strcmp(ip_version, "ipv6"))
        sock->settings.ai_family = AF_INET6;
    else
        sock->settings.ai_family = AF_UNSPEC;

    if(0 == strcmp(protocol, "TCP") || 0 == strcmp(protocol, "tcp"))
    {
        sock->settings.ai_socktype = SOCK_STREAM;
        sock->socktype = TCP;
    }
    else if(0 == strcmp(protocol, "UDP") || 0 == strcmp(protocol, "udp"))
    {
        sock->settings.ai_socktype = SOCK_DGRAM;
        sock->socktype = UDP;
    }
    else
    {
        return 1;
    }


    sock->settings.ai_flags = AI_CANONNAME;

    int status;

    if(0 != (status = getaddrinfo(ip_address, port, &(sock->settings),
                    &sock->serverinfo)))
    {
        return 1;
    }

    return 0;
}

// if 1, then bind, else connect
int bind_sock(Sock * const sock, const unsigned mode)
{
    if(NULL == sock) return -1;

    int returned;
    struct addrinfo * traveler;

    for(traveler = sock->serverinfo; NULL != traveler; traveler = traveler->ai_next)
    {
        if(-1 == (returned = socket(traveler->ai_family, traveler->ai_socktype,
                        traveler->ai_protocol)))
            continue; // fail to get socket, go on to the next

        if(_BIND == mode)
        {
            if(0 == bind(returned, traveler->ai_addr, traveler->ai_addrlen))
                break; // success; we're done here
        }
        else
        {
            if(0 == connect(returned, traveler->ai_addr, traveler->ai_addrlen))
                break; // success; we're done
        }

        close(returned); // couldn't bind, close the socket, try the next 
    }

    if(NULL == traveler)
    {
        // problem binding socket
        freeaddrinfo(sock->serverinfo);
        return -1;
    }

    freeaddrinfo(sock->serverinfo);
    
    sock->socketfd = returned; // record the socket in sock
    
    return 0;
}

// sockfd must refer to a socket of type SOCK_STREAM (tcp) or SOCK_SEQPACKET
// return 0 for success, -1 for error (errno set)
int listening_sock(Sock * const sock)
{
    if(NULL == sock) return -1;

    int returned;

    if(-1 == (returned = listen(sock->socketfd, QUEUE_SIZE)))
    {
        wash_sock(sock);
        return -1;
    }

    return 0;
}

int accept_socks(Sock * const sock, Sock * const new_sock)
{
    if(NULL == sock)
        return -1;

    if(NULL == new_sock)
        return -2;

    new_sock->settings.ai_addrlen = sizeof(*new_sock->settings.ai_addr);

    if(-1 == (new_sock->socketfd = accept(sock->socketfd, 
                    new_sock->settings.ai_addr, &new_sock->settings.ai_addrlen)))
    {
        wash_sock(sock);
        return -3;
    }

    return 0;
}

ssize_t sock_send(Sock * const sock, const char * const message, const size_t length)
{
    if(NULL == sock)
        return -2;

    if(NULL == message)
        return -3;

    ssize_t returned;

    // XXX flag support later
    if(-1 == (returned = send(sock->socketfd, message, length, 0)))
    {
        return -1;
    }
    
    return returned;
}

ssize_t sock_recv(Sock * const sock, char * const buffer, const size_t size)
{
    if(NULL == sock)
        return -2;
    
    if(NULL == buffer)
        return -3;

    ssize_t returned;

    // XXX flag support later
    if(-1 == (returned = recv(sock->socketfd, buffer, size, 0)))
    {
        return -1;
    }

    return returned;
}
