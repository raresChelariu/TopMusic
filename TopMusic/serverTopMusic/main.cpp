#include "server.h"

int main()
{
    Server s;
    int cd;
    s.loadSongs();
    
    while (1)
    {

        fflush(stdout);
        cd = s.acceptClient();
        if (-1 == cd)
        {
            printf("Error on accepting new client!");
            fflush(stdout);
            continue;
        }        
        s.createThread(cd);
    }
    return 0;
}