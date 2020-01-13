#include "song.h"
bool sortByGenre(const Song &a, const Song &b)
{
    return a.genre < b.genre;
}
bool sortByTitle(const Song &a, const Song &b)
{
    return a.title < b.title;
}
bool sortByArtist(const Song &a, const Song &b)
{
    return a.artist < b.artist;
}