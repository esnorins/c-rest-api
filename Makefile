CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -g0 -Os 
LDLIBS := -lulfius -ljansson

all: rest_api

clean: rest_api
	rm $^

rest_api: main.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)
