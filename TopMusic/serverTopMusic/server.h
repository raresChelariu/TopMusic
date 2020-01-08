#pragma once
#include "pch.h"
#include "threadAdaptor.h"
#define NO_MAX_THREADS 100
#define LISTEN_TRIES 30
#define PORT 4000
using namespace std;
class Server
{
private:
    sockaddr_in sockS, sockC;
    int sd;
    int noThreads{0};
    vector<Song> allSongs;
    pthread_t threads[NO_MAX_THREADS];
    long long timeOfLastUpdate{0};
public:
    Server();
    int acceptClient();
    void createThread(int clientDescriptor);
    void loadSongs();
    int getGenre(char* genre);
};

Server::Server()
{
    memset(threads, 0, sizeof(pthread_t)*NO_MAX_THREADS);
    memset(&this->sockS, 0, sizeof(sockaddr_in));
    memset(&this->sockC, 0, sizeof(sockaddr_in));

    sockS.sin_family = AF_INET;
    sockS.sin_port = htons(PORT);
    sockS.sin_addr.s_addr = htonl(INADDR_ANY);

    if (-1 == (sd = socket(AF_INET, SOCK_STREAM, 0)))
    {
        printf("Error on creating socket!\n");
        exit(0);
    }
    int optionReuse = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optionReuse, 4);
    if (-1 == bind(this->sd, (sockaddr*)&this->sockS, sizeof(sockaddr)))
    {
        printf("Error on attempting to bind socket descriptor to local address!\n");
        exit(0);
    }
    if (-1 == listen(sd, LISTEN_TRIES))
    {
        printf("Error on attempting to listen for new connections!\n");
        exit(0);
    }
    printf("Ready to listen\n");
    fflush(stdout);
}
int Server::acceptClient()
{
    sockaddr_in clientSocket;
    memset(&clientSocket, 0, sizeof(sockaddr_in));

    int sizeClientSocket = sizeof(clientSocket);
    int cd = accept(this->sd, (sockaddr*)&clientSocket, (socklen_t*)&sizeClientSocket);
    printf("cd from accept");
    return cd;
}
void Server::createThread(int clientDescriptor)
{
    ThreadAdaptor* th = new ThreadAdaptor(&this->threads[this->noThreads++], clientDescriptor, &this->timeOfLastUpdate, &allSongs);
}
void Server::loadSongs()
{
    FILE* fin = fopen("appsongs.txt", "r");
    char line[256];
    Song s;
    string str;
    char* p;

    while (fgets(line, 256, fin) != NULL)
    {
        *strchr(line, '\n') = 0;

        p = strtok(line, ";");
        s.title = string(p);

        p = strtok(NULL, ";");
        s.artist = string(p);

        p = strtok(NULL, ";");
        s.genre = getGenre(p);


        p = strtok(NULL, ";");
        s.description = string(p);


        p = strtok(NULL, ";");
        s.URL = string(p);

        p = strtok(NULL, ";");
        s.noVotes = atoi(p);

        allSongs.push_back(s);
        printf("%s\n", s.ToString().c_str());
        fflush(stdout);
    }
    printf("%zu", allSongs.size());
    fclose(fin);
}

int Server::getGenre(char* genre)
{
    if (!strcmp(genre, "Blues"))
        return Song::Blues;
    if (!strcmp(genre, "DnB"))
        return Song::DnB;
    if (!strcmp(genre, "HipHop"))
        return Song::HipHop;
    if (!strcmp(genre, "House"))
        return Song::House;
    if (!strcmp(genre, "Jazz"))
        return Song::Jazz;
    if (!strcmp(genre, "Rock"))
        return Song::Rock;
}