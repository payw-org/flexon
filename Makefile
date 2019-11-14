all: flexon

flexon: flexon.lex.o flexon.tab.o flexon_utils.o
	gcc -g -o $@ flexon.tab.o flexon.lex.o flexon_utils.o -lm

flexon.lex.o: flexon.lex.c flexon.tab.h flexon.h

flexon.tab.o: flexon.tab.c flexon.h

flexon.lex.c: flexon.l
	flex --outfile=flexon.lex.c flexon.l

flexon.tab.c flexon.tab.h: flexon.y
	bison -vd flexon.y

clean:
	rm -rf flexon flexon.tab.* flexon.lex.* flexon.output *.o

