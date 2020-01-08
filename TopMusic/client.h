#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <sys/time.h>
#include <netinet/in.h>

class Client
{
public:
    sockaddr_in sock;
    int sd;
    Client();
    void connectToServer();
};
