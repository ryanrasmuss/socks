#ifndef socks_h__
#define socks_h__

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

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

typedef struct sock Sock;

int wash_sock(Sock * const sock);

int set_serverinfo(Sock * const sock, const char * const ip_address, const char * const port);

int make_socket(Sock * const sock);

int bind_socket(Sock * const sock);

int listening_sock(Sock * const sock);

int accept_socks(Sock * const sock, Sock * const new_sock);

int connect_socks(Sock * const sock);

// Functions below do not wash sock on failure

int sock_send(Sock * const sock, const char * const message, const size_t length);

int sock_recv(Sock * const sock, char * const buffer, const size_t size);

#endif // socks_h__
