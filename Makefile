sources = tcp_server.c tcp_client.c udp_recv.c udp_send.c server.c client.c

srcs_dir = test-src

binaries = $(sources:.c=)

LIBR = socks
CURRENT_DIR = $(shell pwd)
SOCKS_DIR = $(CURRENT_DIR)/socks

all: $(SOCKS_LIBR) $(binaries)

CC = gcc
CFLAGS = -Wall

INCLUDES = -I$(SOCKS_DIR) -L$(SOCKS_DIR)

$(SOCKS_LIBR):
	$(MAKE) -C $(SOCKS_DIR)

$(binaries): %: $(srcs_dir)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $< -l$(LIBR)

clean:
	$(MAKE) -C $(SOCKS_DIR) clean
	rm -v $(binaries)
