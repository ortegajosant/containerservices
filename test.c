#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

int main() {

    char palabra[] = "YoSoyElQueVaATerminarEsto";

    printf("%s", &palabra[0,2]);

}