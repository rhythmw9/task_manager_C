CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(shell find . -maxdepth 1 -type f -perm +111 ! -name "*.c" ! -name "*.cpp" ! -name "Makefile")
