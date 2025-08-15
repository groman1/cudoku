main:
	gcc src/main.c src/rawtui.c -o cudoku
install:
	mv cudoku /usr/bin/
debug:
	gcc src/main.c src/rawtui.c -o cudoku-g -g
