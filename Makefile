TARGETS=server client
all: ${TARGETS}

server: server.o fileIO.o
	g++ -o server server.o fileIO.o -D_REETRANT -lpthread

client: client.o
	g++ -o client client.o -D_REETRANT -lpthread

server.o: server.c
	g++ -c -g server.c  -D_REETRANT -lpthread

fileIO.o: fileIO.cc
	g++ -c -g fileIO.cc  -D_REETRANT -lpthread

client.o: client.c
	g++ -c -g client.c  -D_REETRANT -lpthread

clean:
	rm -f *.o server client
