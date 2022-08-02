#pragma once

// Implemented by Andie Pearce

#include <string>
using namespace std;

struct Book
{
    string title, author, rating;
    int pageCount;
    long long int isbn;

    Book()
    {
        title = "";
        author = "";
        isbn = 0;
        pageCount = 0;
        rating = "";
    }
    Book(string title, string author, long long int isbn, string rating, int pageCount)
    {
        this->title = title;
        this->author = author;
        this->isbn = isbn;
        this->pageCount = pageCount;
        this->rating = rating;
    }
};
