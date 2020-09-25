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

# Compile client
client: client.o
	gcc -o client client.o

client.o: client.c
	gcc -c client.c

# Clean object files
clean:
	rm *.o