#pragma once
#include <string>
#include <string.h>
using namespace std;

char typeString[][8] = {"Blues", "DnB", "HipHop", "House", "Jazz", "Rock"};
class Song
{
public:
    enum Type {
        Blues, DnB, HipHop, House, Jazz, Rock
    };
    Song();
    Song(char * strSong);
    string ToString();
    static int getGenre(char * );
    string title, artist, description, URL;
    int genre;
    int noVotes{0};
};
Song::Song() { }

Song::Song(char * strSong)
{
    char * p;
    p = strtok(strSong, ";");
    this->title = string(p);
    
    p = strtok(NULL, ";");
    this->artist = string(p);

    p = strtok(NULL, ";");
    this->genre = Song::getGenre(p);
    
    p = strtok(NULL, ";");
    this->description = string(p);

    p = strtok(NULL, ";");
    this->URL = string(p);

    p = strtok(NULL, ";");
    this->noVotes = (p != NULL ? atoi(p) : 0);
}
string Song::ToString()
{
    string rez;
    rez.append(title);
    rez.append(";");
    rez.append(artist);
    rez.append(";");
    rez.append(typeString[genre]);
    rez.append(";");
    rez.append(description);
    rez.append(";");
    rez.append(URL);
    rez.append(";");
    char str[100];
    sprintf(str, "%d", noVotes);
    rez.append(string(str));
    rez.append(";");

    return rez;
}
int Song::getGenre(char* genre)
{
    int i;
    for (i = 0; i < 7; i++)
        if (!strcmp(genre, typeString[i]))
            return i;
    printf("BAD GENRE!!!! %s !!!!", genre);
    fflush(stdout);
    return -1;
}