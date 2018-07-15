#ifndef socks_h__
#define socks_h__

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string.h> // memset
#include <unistd.h> // close

#define QUEUE_SIZE 5

#define _CONN 0
#define _BIND 1

#define TCP 0
#define UDP 1

// XXX: pack this properly later
struct sock 
{
    struct addrinfo settings;
    struct addrinfo * serverinfo;
    unsigned socktype;
    int socketfd;
    
};

typedef struct sock Sock;

/* return 0 on success, error otherwise */
int wash_sock(Sock * const sock);

/* fill sock with relevant informaiton */
/* return 0 on success, error otherwise */
int fill_sock(Sock * const sock, const char * const ip_address, const char * const port, const char * const ip_version, const char * const protocol);

/* bind sock; for server's udp or tcp */
/* type == 1 then bind (server), else connect (client) */
/* return 0 for success, -1 otherwise; fills socketfd */
int bind_sock(Sock * const sock, const unsigned mode);

/* return 0 on success, error otherwise; tcp only */
int listening_sock(Sock * const sock);

/* return 0 on success, error otherwise; tcp only */
int accept_socks(Sock * const sock, Sock * const new_sock);

// Functions below do not wash sock on failure
// returns 0 if connection ends, positive for success (read bytes), neg if error
ssize_t sock_send(Sock * const sock, const char * const message, const size_t length);

// returns 0 if connection ends, positive for success (read bytes), neg if error
ssize_t sock_recv(Sock * const sock, char * const buffer, const size_t size);

#endif // socks_h__
