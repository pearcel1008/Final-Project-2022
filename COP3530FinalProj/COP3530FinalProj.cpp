#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <fstream>
#include "HashTable.h"
#include "RedBlackTree.h"

using namespace std;

int main()
{
    Hash h;             // Hash map for title and rating search
    RedBlackTree t;     // Red-black tree for title search
    RedBlackTree r;     // Red-black tree for rating search

    cout << "==================================================" << endl;
    cout << "======== Welcome to the Summer Book Club! ========" << endl;
    cout << "=========== BY : Sara, Andie, Carolina ===========" << endl;
    cout << "==================================================" << endl;
    cout << "Loading Structures..." << endl;

    chrono::duration<double> hTotal = (chrono::duration<double>)0;
    chrono::duration<double> tTotal = (chrono::duration<double>)0;

    // Insert nodes into red-black tree 'r' to represent ratings 0-5
    auto start = chrono::system_clock::now();
    for (int i = 0; i < 6; i++)
    {
        r.insertRating(i);
    }
    auto end = chrono::system_clock::now();
    tTotal = (end - start);

    ifstream bookFile("Final_100k.csv");

    // Read data from CSV
    if (bookFile.is_open())
    {
        // Throw away headline data
        string fileLine;
        getline(bookFile, fileLine); 

        while (getline(bookFile, fileLine))
        {
            string title, author, rating;
            int pages;
            long long int isbn;
            istringstream stream(fileLine);

            string tempISBN;
            string tempPages;

            getline(stream, title, ',');
            getline(stream, author, ',');
            getline(stream, tempISBN, ',');
            isbn = stoll(tempISBN, nullptr, 10);
            getline(stream, rating, ',');
            getline(stream, tempPages, ',');
            pages = stoi(tempPages);

            // Create book object from data
            Book* b = new Book(title, author, isbn, rating, pages);

            start = chrono::system_clock::now();
            // Insert into hash table
            h.Insert(b);
            end = chrono::system_clock::now();
            hTotal += (end - start);

            start = chrono::system_clock::now();
            // Insert into red-black tree for titles
            t.Insert(t.head, b);
            // Insert into red-black tree for ranks
            r.pushBook(r.head, b);
            end = chrono::system_clock::now();
            tTotal += (end - start);
        }
    }
    cout << "Inserting into the Hash Table took: " << hTotal.count() << " seconds" << endl;
    cout << "Inserting into the Red Black Tree took: " << tTotal.count() << " seconds" << "\n" << endl;

    // Menu display and functionality 
    int input = -1;
    while (input != 0)
    {
        cout << "******************************" << endl;
        cout << "What would you like to do?" << endl;
        cout << "1. Search by Keyword" << endl;
        cout << "2. Search by Exact Title" << endl;
        cout << "3. Search by Rating" << endl;
        cout << "0. Exit" << endl;
        cout << "******************************" << "\n" << endl;
        
        cin >> input;
        string s, l;

        switch (input)
        {
        case 1:
            cout << "Please enter a single keyword to search: ";
            cin >> s;

            if (h.PartialSearch(s).empty() == true)
            {
                cout << "No Books found matching input criteria." << "\n" << endl;
                break;
            }
            else
            {
                auto start = chrono::system_clock::now();
                h.PartialRatingsHelper(h.PartialSearch(s));
                auto end = chrono::system_clock::now();
                chrono::duration<double> diff = (end - start);
                cout << "Time to search for and print books matching '" << s << "' in hash table: " << diff.count() << " seconds." << "\n" << endl;

                start = chrono::system_clock::now();
                t.searchPartial(t.head, s);
                t.printMenu();
                end = chrono::system_clock::now();
                diff = (end - start);
                cout << "Time to search for and print books matching '" << s << "' in red black tree: " << diff.count() << " seconds." << "\n" << endl;
            }
            break;
        case 2:
            cout << "Please enter an exact title to search: ";
            cin.ignore();
            getline(cin, l);

            if (h.ExactSearch(l) == nullptr)
            {
                cout << "No Books found matching input criteria." << "\n" << endl;
                break;
            }
            else
            {
                auto start = chrono::system_clock::now();
                h.ExactHelper(h.ExactSearch(l));
                auto end = chrono::system_clock::now();
                chrono::duration<double> diff = (end - start);
                cout << "\n" << "Time to search for book with the specified title in hash table: " << diff.count() << " seconds." << "\n" << endl;

                start = chrono::system_clock::now();
                t.searchForTitle(t.head, l);
                t.printMenu();
                end = chrono::system_clock::now();
                diff = (end - start);
                cout << "\n" << "Time to search for book with the specified title in red black tree: " << diff.count() << " seconds." << "\n" << endl;
            }
            break;
        case 3:
            cout << "Please enter a minimum rating to search [0-5]: ";
            cin >> s;

            if (h.RatingsSearch(s).empty() == true)
            {
                cout << "No Books found matching input criteria." << "\n" << endl;
                break;
            }
            else
            {
                auto start = chrono::system_clock::now();
                h.PartialRatingsHelper(h.RatingsSearch(s));
                auto end = chrono::system_clock::now();
                chrono::duration<double> diff = (end - start);
                cout << "\n" << "Time to search for books with the specified minimum rating in hash table: " << diff.count() << "\n" << endl;

                start = chrono::system_clock::now();
                r.printRating(r.head, stoi(s));
                end = chrono::system_clock::now();
                diff = (end - start);
                cout << "\n" << "Time to search for books with the specified minimum rating in red black tree: " << diff.count() << "\n" << endl;
            }
            break;
        case 0:
            cout << "Would you like to generate a reading list before you go? [Y/N]" << endl;
            cin >> s;

            if (s == "N")
            {
                cout << "Exiting The Summer Book Club..." << endl;
            }
            else if (s == "Y")
            {
                cout << "Generating your list!" << endl;
                h.GenerateTXT();
                cout << "Exiting The Summer Book Club..." << endl;
            }
            return 0;
        default:
            cout << "Invalid entry. Please try again." << "\n" << endl;
            break;
        }
    }
}