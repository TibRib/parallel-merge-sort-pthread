seq: seq.o helper.o
	gcc -o seq seq.o helper.o

seq.o: seq.c helper.h
	gcc -c -Wall seq.c

helper.o: helper.c helper.h
	gcc -c -Wall helper.c

clean:
	rm *.o
