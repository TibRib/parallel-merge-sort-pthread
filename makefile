seq: seq.o helper.o
	gcc -o seq seq.o helper.o -ansi

seq.o: seq.c helper.h
	gcc -c -Wall -pedantic seq.c -ansi

helper.o: helper.c helper.h
	gcc -c -Wall -pedantic helper.c -ansi

clean:
	rm *.o
