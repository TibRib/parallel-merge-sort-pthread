seq: seq.o helper.o
	gcc -o seq seq.o helper.o

seq.o: seq.c helper.h
	gcc -c -Wall -pedantic seq.c

helper.o: helper.c helper.h
	gcc -c -Wall -pedantic helper.c

clean:
	rm *.o
