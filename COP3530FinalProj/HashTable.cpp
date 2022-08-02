// HashTable.cpp and HashTable.h Implemented by Larissa "Andie" Pearce 

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include "HashTable.h"
using namespace std;

#define NUM_BUCKETS 50

Hash::Hash()
{
    // All structures given a size of 50
    titles.resize(NUM_BUCKETS);
    ratings.resize(NUM_BUCKETS);
    keywords.resize(NUM_BUCKETS);
}

int Hash::H(string key)
{
    // Creates hash code out of lowercase title
    int sum = 0;
    for (int i = 0; i < key.length(); i++)
    {
        key[i] = tolower(key[i]);
        sum += (int)key[i];
    }
    return sum;
}

int Hash::R(int hCode)
{
    // Creates index from hash code
    return hCode % NUM_BUCKETS;
}

string Hash::Format(string word)
{
    // Removes special characters from passed-in string
    for (int i = 0; i < word.length(); i++)
    {
        if (!isalnum(word.at(i)))
        {
            word.erase(i);
        }
    }
    return word;
}

vector<string> Hash::BreakApartTitle(string title)
{
    // Breaks title into individual keywords
    vector<string> words;
    string temp;

    stringstream stream(title);

    while (getline(stream, temp, ' '))
    {
        temp = Format(temp);
        if (temp != "" && temp != " " && (count(words.begin(), words.end(), temp) == 0))
        {
            words.push_back(temp);
        }
    }
    return words;
}

void Hash::Insert(Book* b)
{
    // Insert title of b

    string t = b->title;
    for (int i = 0; i < t.length(); i++)
    {
        t[i] = tolower(t[i]);
    }
    int tIndex = R(H(t));

    // Titles guaranteed unique in dataset

    titles[tIndex].push_back(make_pair(t, b));

    // Insert rating of b
        // If the rating already exists in the structure, simply append said rating's vector of Books

    int rIndex = R(H(b->rating));
    bool exists = false;
    for (int i = 0; i < ratings[rIndex].size(); i++)
    {
        if (ratings[rIndex][i].first == b->rating)
        {
            exists = true;
            ratings[rIndex][i].second.push_back(b);
            break;
        }
    }
    if (exists == false)
    {
        vector<Book*> v;
        v.push_back(b);
        ratings[rIndex].push_back(make_pair(b->rating, v));
    }

    // Insert keywords from each title; same logic as inserting ratings

    vector<string> w = BreakApartTitle(t);
    for (int i = 0; i < w.size(); i++)
    {
        int kIndex = R(H(w[i]));
        exists = false;
        for (int j = 0; j < keywords[kIndex].size(); j++)
        {
            if (keywords[kIndex][j].first == w[i])
            {
                exists = true;
                keywords[kIndex][j].second.push_back(b);
                break;
            }
        }
        if (exists == false)
        {
            vector<Book*> v_;
            v_.push_back(b);
            keywords[kIndex].push_back(make_pair(w[i], v_));
        }
    }
}

Book* Hash::ExactSearch(string key)
{
    // Calculate index of passed-in title
    for (int i = 0; i < key.length(); i++)
    {
        key[i] = tolower(key[i]);
    }
    int index = R(H(key));
    for (int i = 0; i < titles[index].size(); i++)
    {
        if (titles[index][i].first == key)
        {
            return titles[index][i].second;
        }
    }
    // returns nullptr if Book doesnt exist in structure
    return nullptr;
}

vector<Book*> Hash::PartialSearch(string key) 
{
    // Searches for keywords within table
    vector<Book*> empty;
    for (int i = 0; i < key.length(); i++)
    {
        key[i] = tolower(key[i]);
    }
    int index = R(H(key));
    for (int i = 0; i < keywords[index].size(); i++)
    {
        if (keywords[index][i].first == key)
        {
            return keywords[index][i].second;
        }
    }
    // returns an empty vector if nothing is found
    return empty;
}

vector<Book*> Hash::RatingsSearch(string key)
{
    // Searches for all books with a rating within a specified range 
    // If input is not 5, search for values >= it but < next number
    vector<Book*> v;
    for (int i = 0; i < ratings.size(); i++)
    {
        if (ratings[i].empty() == false)
        {
            for (int j = 0; j < ratings[i].size(); j++)
            {   
                if (stof(key) == 5)
                {
                    if (stof(ratings[i][j].first) == stof(key))
                    {
                        for (int k = 0; k < ratings[i][j].second.size(); k++)
                        {
                            v.push_back(ratings[i][j].second[k]);
                        }
                        return v;
                    }
                }
                else 
                {
                    if (stof(ratings[i][j].first) >= stof(key) && stof(ratings[i][j].first) < (stof(key) + 1))
                    {
                        for (int k = 0; k < ratings[i][j].second.size(); k++)
                        {
                            v.push_back(ratings[i][j].second[k]);
                        }
                    }
                }
            }
        }
    }
    return v;
}

void Hash::PartialRatingsHelper(vector<Book*> v)
{
    // Prints data of found books
    // Sorts based off rating first, then uses ISBN as a tie breaker
    sort(v.begin(), v.end(), [](Book* a, Book* b) { if (stof(a->rating) == stof(b->rating)) { return a->isbn < b->isbn; } return (stof(a->rating) > stof(b->rating)); });
    for (int i = 0; i < 10; i++) // only print first thousand for the sake of time?
    {
        if (i == v.size()) { break; }

        output.push_back(v[i]);
        cout << "---------------------------" << endl;
        cout << "Title: " << v[i]->title << endl;
        cout << "Author: " << v[i]->author << endl;
        cout << "ISBN: " << v[i]->isbn << endl;
        cout << "PageCount: " << v[i]->pageCount << endl;
        cout << "Rating: " << v[i]->rating << "/5" << endl;
    }
}

void Hash::ExactHelper(Book* b)
{
    // Prints data of found book
    output.push_back(b);

    cout << "---------------------------" << endl;
    cout << "Title: " << b->title << endl;
    cout << "Author: " << b->author << endl;
    cout << "ISBN: " << b->isbn << endl;
    cout << "PageCount: " << b->pageCount << endl;
    cout << "Rating: " << b->rating << "/5" << endl;
    cout << endl;
}

void Hash::GenerateTXT()
{
    // Generates a txt file of the user's 10 highest rated books based off search history
    sort(output.begin(), output.end(), [](Book* a, Book* b) { if (stof(a->rating) == stof(b->rating)) { return a->isbn < b->isbn; } return (stof(a->rating) > stof(b->rating)); });
    ofstream myfile;
    myfile.open("ReadingList.txt");
    myfile << "************ Top 10 Recommendations Based Off Search History ************" << endl;

    for (int i = 0; i < 10; i++)
    {
        if (i == output.size()) { break; }
        myfile << "---------------------------" << endl;
        myfile << "Title: " << output[i]->title << endl;
        myfile << "Author: " << output[i]->author << endl;
        myfile << "ISBN: " << output[i]->isbn << endl;
        myfile << "PageCount: " << output[i]->pageCount << endl;
        myfile << "Rating: " << output[i]->rating << "/5" << endl;
        myfile << endl;
    }
    myfile.close();
}