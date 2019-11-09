all: flexon

flexon.tab.c flexon.tab.h: flexon.y
	bison -d flexon.y

lex.yy.c: flexon.l flexon.tab.h
	flex flexon.l

flexon: lex.yy.c flexon.tab.c flexon.tab.h
	gcc -o flexon flexon.tab.c lex.yy.c

clean:
	rm -rf flexon flexon.tab.c lex.yy.c flexon.tab.h

