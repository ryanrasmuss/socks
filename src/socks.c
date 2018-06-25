#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// close
#include <unistd.h>

// debugging 
#include <string.h>
#include <errno.h>

#define UDP 0
#define TCP 1

#define IPV4 0
#define IPV6 1

#define QUEUE_SIZE 5

// XXX: pack this properly later
struct sock 
{
    struct addrinfo settings;
    struct addrinfo * serverinfo;
    char * ip_address;
    char * port_number;
    unsigned protocol;
    unsigned ip_version;
    unsigned flags;
    int socketfd;
};

struct peer_sock
{
    struct sockaddr * addr;
    socklen_t addrlen;
    int socketfd;
};

typedef struct sock Sock;
typedef struct peer_sock Peer_Sock;

// XXX
// combine set_addrinfo and set_servinfo
//
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
    sock->settings.ai_flags    = AI_PASSIVE;

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

int accept_socks(Sock * const sock, Peer_Sock * new_sock)
{
    new_sock->addrlen = sizeof(*new_sock->addr);

    if(-1 == (new_sock->socketfd = accept(sock->socketfd, new_sock->addr, 
                    &new_sock->addrlen)))
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
