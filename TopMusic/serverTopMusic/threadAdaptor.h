#pragma once
#include "threadRuntime.h"
class ThreadAdaptor
{
public:
    class ThreadData
    {
        public:
            int descriptor;
            long long* time;
            vector<Song>* allSongs;
            pthread_mutex_t * lock;
    };
    ThreadAdaptor(pthread_t* threadIdentifier, int descriptor, long long* time, vector<Song>* songs, pthread_mutex_t * lock);
    static void* ThreadMain(void*);
};
ThreadAdaptor::ThreadAdaptor(pthread_t* threadIdentifier, int descriptor, long long* time, vector<Song>* songs, pthread_mutex_t * lock)
{
    fflush(stdout);
    ThreadData * data = (ThreadData*) malloc(sizeof(ThreadData));
    data->descriptor = descriptor;
    data->time = time;
    data->allSongs = songs;
    data->lock = lock;
    printf("\n@@@%zu@@@\n", (*songs).size());
    fflush(stdout);
    pthread_create(threadIdentifier, NULL, this->ThreadMain, (void*)data);
}
void* ThreadAdaptor::ThreadMain(void* arg)
{
    fflush(stdout);
    ThreadData data = *((ThreadData*)arg);
    
    ThreadRuntime th(data.descriptor, data.time, data.allSongs, data.lock);   
    printf("~~~%zu~~~\n", (*data.allSongs).size());
    fflush(stdout);
    while (1)
    {
        fflush(stdout);
        if (th.Login())
            break;
    }
    while (1)
    {
        if (-1 == th.ClientAction())
            return NULL;
    }
    pthread_cancel(pthread_self());
    return NULL;
}
