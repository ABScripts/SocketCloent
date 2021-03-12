a.out: main.o
	@g++ -o a.out  main.o socket.o unixsock.o winsock.o server.o connection.o peermanager.o logger.o -pthread

main.o: server.o 
	@g++ -c src/main.cpp

server.o: peermanager.o connection.o logger.o
	@g++ -c src/server.cpp

peermanager.o:
	@g++ -c src/peermanager.cpp

connection.o: socket.o
	@g++ -c src/connection.cpp

socket.o: unixsock.o winsock.o
	@g++ -c src/network/socket.cpp

unixsock.o:
	@g++ -c src/network/unixsock.cpp

winsock.o:
	@g++ -c src/network/winsock.cpp

logger.o:
	@g++ -c src/logger.cpp

clean:
	@rm socket.o unixsock.o winsock.o a.out server.o connection.o main.o logger.o
