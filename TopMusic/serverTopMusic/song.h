#pragma once
#include <string>
using namespace std;

char typeString[][8] = {"Blues", "DnB", "HipHop", "House", "Jazz", "Rock"};
class Song
{
public:
    enum Type {
        Blues, DnB, HipHop, House, Jazz, Rock
    };
    
    string ToString();
    string title, artist, description, URL;
    int genre;
    int noVotes{0};
};
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