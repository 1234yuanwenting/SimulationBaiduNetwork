all :ser

GDB=-g
ser:ser.o thread.o sock_init.o
	gcc -o ser ser.o thread.o sock_init.o -lpthread

ser.o : ser.c
	gcc -c ser.c ${GDB}

thread.o : thread.c
	gcc -c thread.c ${GDB}

sock_init.o : sock_init.c
	gcc -c sock_init.c ${GDB}

clean:
	rm -f ser *.o
