# SOCKS

Simple socket library. This is the backbone to another project. The goal is to quickly create ``tcp`` and ``udp`` connections.

## Overview

### Sock Servers

To build a server socket (expects a new connection):

1. Call ``fill_sock`` with ip address, port, ip version, and protocol (udp or tcp).
2. Call ``bind_sock`` with ``mode == _BIND`` 

TCP Servers require the following extra steps

1. ``listening_sock`` with default ``QUEUE_SIZE = 5`` (you may edit this in ``socks.h``)
2. ``accept_socks`` which will fill the second parameter ``new_sock`` with data from the newly connected client.

For udp you do not need ``listening_sock`` and ``accept_socks``.

### Sock Clients

1. Call ``fill_sock`` with ip address, port, ip version, and protocol (udp or tcp)
2. Call ``bind_sock`` with ``mode == _CONN``

Refer to ``test-src/*`` for example implementations.


### Compiling Notes

```gcc -Iincludes/ -Ldir/to/library/ -o example example.c -ldir/to/object```


