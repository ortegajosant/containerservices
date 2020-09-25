#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "color_detection.c"

#define PORT 8080

// Run the server that manage the connection
int run_server()
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char *buffer;

    buffer = (char *)malloc(1024);
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    int connected = 0;

    memset(buffer, 0, 1024);
    char file_name[100];
    while (1)
    {
        printf("Listen, connected: %d\n", connected);
        if (connected == 0)
        {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                     (socklen_t *)&addrlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            connected = 1;
        }
        else if (connected == 1)
        {

            valread = recv(new_socket, file_name, 100, 0);
            if (strcmp(file_name, "exit") <= 0)
            {
                char bye[] = "Bye!";
                send(new_socket, bye, strlen(bye), 0);
                printf("From client: %s\n", file_name);
                connected = 0;
            }
            else
            {
                send(new_socket, file_name, 100, 0);

                long buff_len;

                valread = recv(new_socket, &buff_len, sizeof(buff_len), 0);

                int chunks = buff_len / 1024;

                int count = 0;

                buffer = (char *)realloc(buffer, sizeof(char) * buff_len);

                printf("Len is: %ld\n", buff_len);

                while (count <= chunks)
                {
                    send(new_socket, &count, sizeof(count), 0);
                    if ((count + 1) * 1024 > buff_len)
                    {
                        valread = recv(new_socket, buffer + (count * 1024), buff_len - count * 1024, 0);
                    }
                    else
                    {
                        valread = recv(new_socket, buffer + (count * 1024), 1024, 0);
                    }
                    printf("Getting chunk: %d\n", count);
                    count++;
                }

                printf("Hello message received \n %s \n\n", buffer);
                send(new_socket, file_name, strlen(file_name), 0);
                detect_color(buffer, buff_len);
                memset(buffer, 0, buff_len);
                memset(file_name, 0, 100);
            }
        }
        else
        {
            continue;
        }
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    struct stat st = {0};
    if (stat("Not trusted", &st) == -1)
    {
        mkdir("Not trusted", 0777);
    }
    if (stat("B", &st) == -1)
    {
        mkdir("B", 0777);
    }
    if (stat("G", &st) == -1)
    {
        mkdir("G", 0777);
    }
    if (stat("R", &st) == -1)
    {
        mkdir("R", 0777);
    }
    run_server();
}