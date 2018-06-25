srcs = server.c test_client.c

src_dir = src

bins = $(srcs:.c=)

CC = gcc

all: $(bins)

$(bins): %: $(src_dir)/%.c
	$(CC) $< -o $@

clean:
	rm -v $(bins)
