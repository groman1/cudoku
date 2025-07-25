main:
	gcc src/main.c -lncurses -o cudoku
install:
	mv cudoku /usr/bin/
debug:
	gcc src/main.c -lncurses -o cudoku-g -g
