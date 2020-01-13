#include "client.h"
#define PORT 4000
#define MYHOST "127.0.0.1"
#include <string.h>
Client::Client()
{
    memset(&sock, 0, sizeof(sockaddr_in));
    sock.sin_family = AF_INET;
    sock.sin_port = htons(PORT);
    sock.sin_addr.s_addr = inet_addr(MYHOST);
}
void Client::connectToServer()
{
    if (-1 == (this->sd = socket(AF_INET, SOCK_STREAM, 0)))
    {
        printf("Error on creating socket!\n");
        return;
    }
    if (-1 == ::connect(this->sd, (sockaddr*)&sock, sizeof(sockaddr)))
    {
        printf("Error on trying to connect to server!\n");
        return;
    }
    printf("Connection was made!");
    fflush(stdout);
}
