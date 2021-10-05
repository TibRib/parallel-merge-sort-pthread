d1p: d1p.o
	gcc -o d1p d1p.o  -lpthread

d1p.o: d1p.c
	gcc -c -Wall d1p.c -lpthread

d1s: d1s.o 
	gcc -o d1s d1s.o

d1s.o: d1s.c 
	gcc -c -Wall d1s.c

clean:
	rm *.o

all: d1p d1s
	echo "ok";
