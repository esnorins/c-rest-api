rest_api: main.c
	clang -o rest_api main.c -Wall -Wextra -Wpedantic -g0 -Os -lulfius -ljansson
