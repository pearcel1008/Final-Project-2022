#pragma once

// Sara Winner and Carolina Verde

#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include "Book.h"

using namespace std;

// SOURCE: https://www.geeksforgeeks.org/c-program-red-black-tree-insertion/
enum Color
{
    RED,
    BLACK
};

struct Node
{
    Book* book;                 // Book object
    vector<Book*> books;        // Vector of book objects

    string data;                // Book title
    int rating;                 // Book rating

    bool color;
    Node* left;
    Node* right;
    Node* parent;

    // Constructor for title search
    Node(string title, Book* book)
    {
        this->data = title;
        this->color = RED;
        this->book = book;
        this->rating = stoi(book->rating);

        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }

    // Constructor for rating search
    Node(int rating) {
        this->rating = rating;
        this->color = RED;

        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};

struct RedBlackTree
{
    Node* head;                                         // Root node
    vector<Book*> listOfBooks;                          // Vector of book objects

    RedBlackTree()
    {
        head = nullptr;
    }

    // Red-black tree creation
    Node* Insert(Node* node, Book* b);                  // Insert book node according to title
    void balanceTree(Node* root, Node* pt);             // Balance red-black tree
    void rightRotate(Node* root, Node* node);           // Right rotate node
    void leftRotate(Node* root, Node* node);            // Left rotate node
    void RBTree(Node* node, Book* b);                   // Insert book node
    void insertRating(const int& data);                 // Insert int variable for rating
    Node* insertNode(Node* root, Node* node);           // Insert node according to rating

    // Red-black tree methods for title search
    void searchForTitle(Node* node, string title);      // Search for exact title
    void searchPartial(Node* node, string title);       // Search for partial title
    void printMenu();                                   // Display title search results
    string removeSpecial(string word);                  // Remove special characters
    string lowerCase(string word);                      // Convert string to lowercase
    vector<string> splitTitle(string word);
    bool inTitle(vector<string> words, vector<string> listOfTitles);

    // Red-black tree methods for rating search
    void pushBook(Node* node, Book* book);              // Push book object into appropriate vector
    void printRating(Node* node, int rating);           // Display rating search results
};

// FUNCTIONALITY: Insert book node according to title
Node* RedBlackTree::Insert(Node* node, Book* b)
{
    // Convert book title to lowercase
    string title = lowerCase(b->title);

    // Create head node if tree is empty
    if (head == nullptr)
    {
        head = new Node(title, b);
        return head;
    }
    // Create book node based on title
    else if (node == nullptr)
    {
        node = new Node(title, b);
    }
    // Insert into left branch
    else if (title < node->data)
    {
        node->left = Insert(node->left, b);
    }
    // Insert into right branch
    else if (title > node->data)
    {
        node->right = Insert(node->right, b);
    }

    return node;
}

// FUNCTIONALITY: Insert node according to rating
Node* RedBlackTree::insertNode(Node* root, Node* node) {
    // If tree is empty
    if (root == nullptr)
    {
        return node;
    }
    else
    {
        // Insert into left branch
        if (node->rating < root->rating)
        {
            root->left = insertNode(root->left, node);
            root->left->parent = root;
        }
        // Insert into right branch
        else if (node->rating > root->rating)
        {
            root->right = insertNode(root->right, node);
            root->right->parent = root;
        }
    }

    return root;
}

// FUNCTIONALITY: Insert node according to rating
void RedBlackTree::insertRating(const int& data) {
    // Create node based on rating
    Node* node = new Node(data);

    // Insert node into red-black tree
    head = insertNode(head, node);

    // Balance red-black tree after insertion
    balanceTree(head, node);
}

// FUNCTIONALITY: Push book object into appropriate vector
void RedBlackTree::pushBook(Node* node, Book* book) {
    // Traverse red-black tree and find correct node
    if (node == nullptr)
    {
        return;
    }
    else if (node->rating > stoi(book->rating))
    {
        pushBook(node->left, book);
    }
    else if (node->rating < stoi(book->rating))
    {
        pushBook(node->right, book);
    }
    // Locate correct node and push book object into vector
    else if (node->rating == stoi(book->rating))
    {
        node->books.push_back(book);
    }
}

// FUNCTIONALITY: Display rating search results
void RedBlackTree::printRating(Node* node, int rating)
{
    // Traverse red-black tree and find correct node
    if (node == nullptr)
    {
        return;
    }
    else if (node->rating > rating)
    {
        printRating(node->left, rating);
    }
    else if (node->rating < rating)
    {
        printRating(node->right, rating);
    }
    // Locate correct node and display search results
    else if (node->rating == rating)
    {
        // Sort vector of book objects
        sort(node->books.begin(), node->books.end(), [](Book* a, Book* b) { if (stof(a->rating) == stof(b->rating)) { return a->isbn < b->isbn; } return (stof(a->rating) > stof(b->rating)); });

        // for (auto i: node->books) {
        for (int i = 0; i < 10; i++) {
            cout << "---------------------------" << endl;
            cout << "Title: " << node->books[i]->title << endl;
            cout << "Author: " << node->books[i]->author << endl;
            cout << "Page Count: " << node->books[i]->pageCount << endl;
            cout << "ISBN: " << node->books[i]->isbn << endl;
            cout << "Rating: " << node->books[i]->rating << "/5" << endl;
        }

        // Clearing vector of book objects for when user uses menu again
        node->books.clear();
    }
}

// SOURCE: Based on pseudocode from Balanced Trees page 171
// FUNCTIONALITY: Balance red-black tree
void RedBlackTree::balanceTree(Node* root, Node* pt) {
    Node* parent_pt = nullptr;
    Node* grand_parent_pt = nullptr;
    Node* uncle_pt = nullptr;

    if ((pt != root) && (pt->color != BLACK))
    {
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        if (pt->parent->color)
        {
            if (parent_pt == grand_parent_pt->left)
            {
                uncle_pt = grand_parent_pt->right;
            }
            else
            {
                uncle_pt = grand_parent_pt->left;
            }

            if (uncle_pt != nullptr && uncle_pt->color == RED)
            {
                parent_pt->color = uncle_pt->color = BLACK;
                grand_parent_pt->color = RED;
                balanceTree(root, grand_parent_pt);

                return;
            }

            if (pt == parent_pt->right && parent_pt == grand_parent_pt->left)
            {
                leftRotate(root, parent_pt);
                pt = parent_pt;
                parent_pt = pt->parent;
            }
            else if (pt == parent_pt->left && parent_pt == grand_parent_pt->right)
            {
                rightRotate(root, parent_pt);
                pt = parent_pt;
                parent_pt = pt->parent;
            }

            parent_pt->color = BLACK;
            grand_parent_pt->color = RED;

            if (pt == parent_pt->left)
            {
                rightRotate(root, grand_parent_pt);
            }
            else
            {
                leftRotate(root, grand_parent_pt);
            }
        }
    }
}

// SOURCE: https://www.geeksforgeeks.org/c-program-red-black-tree-insertion/
// FUNCTIONALITY: Right rotate node
void RedBlackTree::rightRotate(Node* root, Node* node)
{
    Node* parent_left = node->left;

    node->left = parent_left->right;

    if (node->left != nullptr)
    {
        node->left->parent = node;
    }

    parent_left->parent = node->parent;

    if (node->parent == nullptr)
    {
        root = parent_left;
    }
    else if (node == node->parent->left)
    {
        node->parent->left = parent_left;
    }
    else
    {
        node->parent->right = parent_left;
    }

    parent_left->right = node;
    node->parent = parent_left;
}

// SOURCE: https://www.geeksforgeeks.org/c-program-red-black-tree-insertion/
// FUNCTIONALITY: Left rotate node
void RedBlackTree::leftRotate(Node* root, Node* node)
{
    Node* parent_right = node->right;

    node->right = parent_right->left;

    if (node->right != nullptr)
    {
        node->right->parent = node;
    }

    parent_right->parent = node->parent;

    if (node->parent == nullptr)
    {
        root = parent_right;
    }
    else if (node == node->parent->left)
    {
        node->parent->left = parent_right;
    }
    else
    {
        node->parent->right = parent_right;
    }

    parent_right->left = node;
    node->parent = parent_right;
}

// FUNCTIONALITY: Insert book node
void RedBlackTree::RBTree(Node* node, Book* b)
{
    // Insert node with book object into red-black tree
    node = Insert(node, b);
    // Balance red-black tree
    balanceTree(head, node);
}

// FUNCTIONALITY: Search for exact title
void RedBlackTree::searchForTitle(Node* node, string title)
{
    // Convert book title to lowercase
    title = lowerCase(title);

    if (node == nullptr)
    {
        return;
    }
    else if (node->data > title)
    {
        searchForTitle(node->left, title);
    }
    else if (node->data < title)
    {
        searchForTitle(node->right, title);
    }
    // Locate correct node and push book object into vector
    else if (node->data == title)
    {
        listOfBooks.push_back(node->book);
    }
}

// FUNCTIONALITY: Display title search results
void RedBlackTree::printMenu()
{
    // Sort vector of book objects
    sort(listOfBooks.begin(), listOfBooks.end(), [](Book* a, Book* b) { if (stof(a->rating) == stof(b->rating)) { return a->isbn < b->isbn; } return (stof(a->rating) > stof(b->rating)); });

    int size;

    if (listOfBooks.size() < 10)
    {
        size = listOfBooks.size();
    }
    else
    {
        size = 10;
    }

    for (int i = 0; i < size; i++)
    {
        cout << "---------------------------" << endl;
        cout << "Title: " << listOfBooks[i]->title << endl;
        cout << "Author: " << listOfBooks[i]->author << endl;
        cout << "Page Count: " << listOfBooks[i]->pageCount << endl;
        cout << "ISBN: " << listOfBooks[i]->isbn << endl;
        cout << "Rating: " << listOfBooks[i]->rating << "/5" << endl;
    }

    // Clearing vector of book objects for when user uses menu again
    listOfBooks.clear();
}

// FUNCTIONALITY: Search for partial title
void RedBlackTree::searchPartial(Node* node, string title)
{
    vector<string> tempTitle;
    vector<string> lowercaseTitle;
    title = lowerCase(title);

    if (node == nullptr)
    {
        return;
    }
    else
    {
        // Convert each title to lowercase
        tempTitle = splitTitle(title);
        lowercaseTitle = splitTitle(node->data);

        if (inTitle(tempTitle, lowercaseTitle))
        {
            listOfBooks.push_back(node->book);
        }

        searchPartial(node->left, title);
        searchPartial(node->right, title);
    }
}

// FUNCTIONALITY: Remove special characters
string RedBlackTree::removeSpecial(string word)
{
    for (int i = 0; i < word.length(); i++)
    {
        if (!isalnum(word.at(i)))
        {
            word.erase(i);
        }
    }

    return word;
}

// FUNCTIONALITY: Convert string to lowercase
string RedBlackTree::lowerCase(string word)
{
    string tempTitle;

    for (int i = 0; i < word.length(); i++)
    {
        char x = word.at(i);
        x = tolower(x);
        tempTitle += x;
    }

    return tempTitle;

}

// FUNCTIONALITY: Split title for partial book title search
vector<string> RedBlackTree::splitTitle(string word) {
    vector<string> newTitle;
    string temp;

    size_t found = word.find(" ");

    // Check if partial word is in title
    if (found == string::npos)
    {
        // If yes, add book object into vector
        word = removeSpecial(word);
        newTitle.push_back(word);
    }
    else
    {
        stringstream stream(word);

        while (getline(stream, temp, ' '))
        {
            temp = removeSpecial(temp);

            if (temp != "" && temp != " " && (count(newTitle.begin(), newTitle.end(), temp) == 0))
            {
                newTitle.push_back(temp);
            }
        }
    }

    return newTitle;
}

// FUNCTIONALITY: Check if partial search exists in book title
bool RedBlackTree::inTitle(vector<string> title, vector<string> listOfTitles) {
    bool inside = false;

    for (int i = 0; i < title.size(); i++)
    {
        for (int j = 0; j < listOfTitles.size(); j++)
        {
            // Check if the partial word is in the title
            if (title[i] == listOfTitles[j])
            {
                inside = true;
                break;
            }
            else
            {
                inside = false;
            }
        }
    }

    return inside;
}
