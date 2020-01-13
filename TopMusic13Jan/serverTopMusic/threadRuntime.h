#pragma once

#include "song.h" // CLASA IN CARE ESTE DEFINITA UN CANTEC
#include "server.h" 
#include "musicSorting.h" // CRITERII DE SORTARE PT STD::SORT
#include "algorithm" // PT STD::SORT

#define FILENAME_SONGS "appsongs.txt"

class ThreadRuntime
{
public:
    bool isAdmin = false;
    bool isLogged = false;

    int cd;
    long long* serverTime;
    vector<Song>* allSongs;
    pthread_mutex_t * lock;
public:
    ThreadRuntime(int descriptor, long long* time, vector<Song>* songs, pthread_mutex_t * lock);
    bool Login();
    int ClientAction();
    void SaveToDisk();
};
ThreadRuntime::ThreadRuntime(int descriptor, long long* time, vector<Song>* songs, pthread_mutex_t * lock)
{
    this->cd = descriptor;
    this->serverTime = time;
    this->allSongs = songs;
    this->lock = lock;
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
    if (!strcmp(msg, "UPDATE_VOTE_NO"))
    {

        int rowNo = 0, votes = 0;
        read(this->cd, &rowNo, 4);
        read(this->cd, &votes, 4);
        pthread_mutex_lock(this->lock);

        (*allSongs).at(rowNo).noVotes = votes;
        this->SaveToDisk();

        pthread_mutex_unlock(this->lock);
    }
    if (!strcmp(msg, "ADD_SONG"))
    {
        printf("Adding SONG!\n");
        fflush(stdout);
        char* strSong = (char*) malloc(256);
        memset(strSong, 0, 256);
        read(this->cd, strSong, 256);

        Song s = Song(strSong);

        pthread_mutex_lock(this->lock);

        (*allSongs).push_back(s);

        pthread_mutex_unlock(this->lock);
        free(strSong);    
        this->SaveToDisk();
    }
    if (!strcmp(msg, "SORT_BY"))
    {
        char* criteriuSortare = (char*) malloc(256);
        memset(criteriuSortare, 0, 256);
        read(this->cd, criteriuSortare, 256);
        pthread_mutex_lock(lock);
        if (!strcmp(criteriuSortare, "GENRE"))
            std::sort(allSongs->begin(), allSongs->end(), sortByGenre);
        else if (!strcmp(criteriuSortare, "TITLE"))
            std::sort(allSongs->begin(), allSongs->end(), sortByTitle);
        else if (!strcmp(criteriuSortare, "ARTIST"))
            std::sort(allSongs->begin(), allSongs->end(), sortByArtist);
        else 
        {
            printf("Wrong command for sorting!!!\n");
            printf("Input sorting ----%s----\n", criteriuSortare);
            fflush(stdout);
            exit(5);
        }
        printf("\n\n\n\n\n\n");
        Song mySong;
        for (auto it= allSongs->begin(); it != allSongs->end(); ++it)
        {
            mySong = *it;
            printf("%s\n",mySong.ToString().c_str());
        }
        printf("\n\n\n\n\n\n");
        fflush(stdout);
        free(criteriuSortare);
        pthread_mutex_unlock(lock);
        this->SaveToDisk();   
    }
    free(msg);
    return 1;
}
void ThreadRuntime::SaveToDisk()
{
    printf("SAVING TO DISK\n");
    fflush(stdout);
    pthread_mutex_lock(lock);
    FILE * fout = fopen(FILENAME_SONGS, "w");
    Song currSong;
    for (auto it = (*allSongs).begin(); it != (*allSongs).end(); ++it)
    {
        currSong = *it;
        fprintf(fout, "%s\n", currSong.ToString().c_str());
    }
    fclose(fout);
    pthread_mutex_unlock(lock);
    printf("FILE SAVED TO DISK\n");
    fflush(stdout);
}