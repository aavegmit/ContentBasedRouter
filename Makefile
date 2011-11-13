TARGETS=server client router node
all: ${TARGETS}

server: server.o fileIO.o
	g++ -o server server.o fileIO.o -D_REETRANT -lpthread

client: client.o fileIO.o
	g++ -o client client.o fileIO.o -D_REETRANT -lpthread

router: router.o
	g++ -o router router.o -D_REETRANT -lpthread

node: node.o
	g++ -o node node.o -D_REETRANT -lpthread

server.o: server.c
	g++ -c -g server.c  -D_REETRANT -lpthread

fileIO.o: fileIO.cc
	g++ -c -g fileIO.cc  -D_REETRANT -lpthread

client.o: client.c
	g++ -c -g client.c  -D_REETRANT -lpthread

router.o: router.c
	g++ -c -g router.c  -D_REETRANT -lpthread

node.o: node.c
	g++ -c -g node.c  -D_REETRANT -lpthread

clean:
	rm -f *.o server client node router
