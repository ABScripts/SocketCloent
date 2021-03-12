a.out: main.o
	@g++ -o a.out  main.o socket.o unixsock.o winsock.o server.o connection.o client.o peermanager.o -pthread

main.o: server.o client.o
	@g++ -c src/main.cpp

client.o: connection.o
	@g++ -c src/client.cpp

server.o: peermanager.o connection.o
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

clean:
	@rm socket.o unixsock.o winsock.o a.out server.o connection.o client.o main.o
