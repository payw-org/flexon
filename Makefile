all: flexon

flexon: flexon.lex.o flexon.tab.o
	gcc -g -o $@ flexon.tab.o flexon.lex.o -lm

flexon.lex.o: flexon.lex.c flexon.tab.h flexon.tab.h

flexon.tab.o: flexon.tab.c flexon.lex.h flexon.h

flexon.lex.c flexon.lex.h: flexon.l
	flex --outfile=flexon.lex.c flexon.l

flexon.tab.c flexon.tab.h: flexon.y
	bison -vd flexon.y

clean:
	rm -rf flexon flexon.tab.* flexon.lex.* flexon.output

