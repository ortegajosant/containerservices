# Start image detection
detection: server
	make server
	make clean
	./server

# Compile server
server: server.o
	gcc -o server server.o -lpng

server.o: server.c
	gcc -c server.c

#Execute client
sendimg:
	make client
	make clean
	./client

containerun:
	docker run -it -p 127.0.0.1:6000:8080 container

# Compile client
client: client.o
	gcc -o client client.o

client.o: client.c
	gcc -c client.c

# Clean object files
clean:
	rm *.o