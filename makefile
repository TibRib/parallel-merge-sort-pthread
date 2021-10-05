par: par.o helper.o
	gcc -o par par.o helper.o -lpthread

par.o: par.c helper.h
	gcc -c -Wall par.c -lpthread

seq: seq.o helper.o
	gcc -o seq seq.o helper.o

seq.o: seq.c helper.h
	gcc -c -Wall seq.c

helper.o: helper.c helper.h
	gcc -c -Wall helper.c

clean:
	rm *.o
