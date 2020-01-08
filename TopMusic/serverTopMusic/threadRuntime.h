#pragma once
#include "song.h"

class ThreadRuntime
{
public:
    bool isAdmin = false;
    bool isLogged = false;

    int cd;
    long long* serverTime;
    vector<Song>* allSongs;

    ThreadRuntime(int descriptor, long long* time, vector<Song>* songs);
    bool Login();
    int ClientAction();
};
ThreadRuntime::ThreadRuntime(int descriptor, long long* time, vector<Song>* songs)
{
    cd = descriptor;
    serverTime = time;
    allSongs = songs;
    printf("^^^%zu^^\n", (*allSongs).size());
    fflush(stdout);
}
bool ThreadRuntime::Login()
{
    char* msg = (char*) malloc(256);
    memset(msg, 0, 256);
    
    read(this->cd, msg, 256);
    perror("a");
    fflush(stdout);
    printf("%d\n", cd);
    char* user = strtok(msg, ";");
    char* pass = strtok(NULL, ";");
    if (!strcmp(user, "admin") && !strcmp(pass, "admin"))
    {
        isLogged = isAdmin = true;       
        strcpy(msg, "LOGIN_VALID");
        write(this->cd, msg, 256);
        return 1;
    }
    char line[256];
    char *currUser, *currPass;
    FILE* fin = fopen("credentials.txt", "r");
    while (fgets(line, 256, fin) != NULL)
    {
        *strchr(line, '\n') = 0; // fgets ia si ENTER in citire
        currUser = strtok(line, ";");
        currPass = strtok(NULL, ";");
        if (!strcmp(user, currUser) && !strcmp(pass, currPass))
        {
            strcpy(msg, "LOGIN_VALID");
            write(this->cd, msg, 256);
            isLogged = true;
            return 1;
        }
    }
    write(this->cd, "LOGIN_INVALID", 256);
    printf("LOGIN_VALID\n");
    fflush(stdout);
    free(msg);
    return 0;
}
int ThreadRuntime::ClientAction()
{
    char* msg = (char*) malloc(256);
    memset(msg, 0, 256);
    if (0 == read(this->cd, msg, 256))
    {

        printf("Connection problem!");
        fflush(stdout);
        return -1;
    }
    printf("&&%s##", msg);
    fflush(stdout);
    vector<Song>::iterator it;
    if (!strcmp(msg, "GET_SONGS"))
    {
        size_t size = (*allSongs).size();
        long long noSongs = static_cast<int>(size);
        write(this->cd, &noSongs, sizeof(noSongs));
        for (it = (*allSongs).begin(); it != (*allSongs).end(); it++)
            write(this->cd, (*it).ToString().c_str(), 256);       
    }   
    free(msg);
    return 1;
}
