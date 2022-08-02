#pragma once

// Implemented by Andie Pearce 

#include <vector>
#include <string>
#include "Book.h"
using namespace std;

class Hash
{
    int H(string key);                            // Hash Function - converts data type to an int
    int R(int hCode);                             // Generates index 
    string Format(string word);                   // Removes epcial characters from title for keyword functionality
    vector<string> BreakApartTitle(string title); // returns vector of individual words to hash and store

public:
    vector<vector<pair<string, Book*>>> titles;   // Stores Books at indices calclated by R(H(title))
    vector<vector<pair<string, vector<Book*>>>> ratings;
    vector<vector<pair<string, vector<Book*>>>> keywords;

    vector<Book*> output;                         // Used to generate a txt upon the program's termination

    Hash();                                       // Constructor
    void Insert(Book* b);                         // Adds books to the structures
    Book* ExactSearch(string key);                // Searches for exact title, including punctuation. Is NOT case sensitive -- 'TWilIGHT' == 'twilight' == 'Twilight'
    vector<Book*> PartialSearch(string key);      // Returns a list of entries containing a given keyword somewhere within their title -> helper function to ber implemented to keep main clean
    vector<Book*> RatingsSearch(string key);      // List of entries at or above an input rating -> helper function coming soon 
    void ExactHelper(Book* b);                    // Print function 
    void PartialRatingsHelper(vector<Book*> v);   // Print Function
    void GenerateTXT();                           // Generates a TXT file
};