#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define PORT 6000

char *buffer;

long read_file(char *file_name)
{
    FILE *fp;
    long filelen;
    fp = fopen(file_name, "rb"); // Open the file in binary mode
    fseek(fp, 0, SEEK_END);      // Jump to the end of the file
    filelen = ftell(fp);
    rewind(fp);
    buffer = (char *)malloc(sizeof(char) * filelen);
    fread(buffer, filelen, 1, fp);
    fclose(fp);
    printf("File : %s\nLen: %ld\n", buffer, filelen);
    return filelen;
}

int run_client(const char * ip[])
{

    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char sv_buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, ip[1], &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    char entry[100];

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    int send_var;
    int recv_var;
    long buff_len;

    while (1)
    {
        printf("\nEnter a valid PNG file name: \n");
        scanf("%s", entry);
        printf("Entry word: %s\n", entry);

        if (strcmp(entry, "exit") == 0)
        {
            send_var = send(sock, hello, strlen(hello), 0);
            recv_var = recv(sock, sv_buffer, 1024, 0);
            printf("Message from server: %s\n", sv_buffer);
            close(sock);
            break;
        }
        else
        {
            buff_len = read_file(entry);

            printf("File is: %s\n", buffer);
            
            send_var = send(sock, entry, 100, 0);

            recv_var = recv(sock, sv_buffer, 100, 0);

            printf("Name received %s\n", sv_buffer);

            send_var = send(sock, &buff_len, sizeof(buff_len), 0);

            printf("Len buff %ld\n", buff_len);

            if (send_var < 0)
            {
                printf("Can't send data\n");
                continue;
            }
            int chunks = buff_len / 1024;
            int count = 0;

            printf("Chunks %d\n", chunks);

            int msg_rsv;

            while (count <= chunks)
            {
                recv_var = recv(sock, &msg_rsv, sizeof(msg_rsv), 0);
                printf("Time to chunk: %d\n", msg_rsv);
                if ((count + 1) * 1024 > buff_len)
                {
                    printf("Final chunk\n");
                    send_var = send(sock, &buffer[count * 1024], buff_len - count * 1024, 0);
                }
                else
                {
                    send_var = send(sock, &buffer[count * 1024], 1024, 0);
                }
                printf("Sending chunk: %d\n", count);

                count++;
            }

            printf("Hello message sent\n");

            recv_var = recv(sock, sv_buffer, 1024, 0);
            if (recv_var < 0)
            {
                continue;
            }
            printf("%s\n", sv_buffer);
            memset(buffer, 0, 1024);
        }
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    run_client(argv);
}
