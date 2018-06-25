sources = test_client.c server.c

binaries = $(sources:.c=)

SOCKS_DIR = socks
SOCKS_LIBR = $(SOCKS_DIR)/$(SOCKS_DIR).a

all: $(SOCKS_LIBR) $(binaries)

CC = gcc
CFLAGS = -Wall -pipe -v

INCLUDE = -I$(SOCKS_DIR)

$(SOCKS_LIBR):
	$(MAKE) -C $(SOCKS_DIR)

$(binaries): %: %.c
	$(CC)  $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	$(MAKE) -C $(SOCKS_DIR) clean
	rm -v $(binaries)
