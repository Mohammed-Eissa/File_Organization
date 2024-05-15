#ifndef BOOK_H
#define BOOK_H
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string.h>
#include <vector>
#include <map>
using namespace std;

struct book
{
    int id;
    char name[50], author[50], category[50];
};

class Book
{
private:
    short header;
    map<int, int> primary;
    map<string, int> secondry;
    int tmp_id;
    string tmp_name;

public:
    Book();
    ~Book();
    void initial();
    void insert(book);
    bool insert_available(book, int);
    void insert_end(book);
    void remove_by_id(int);
    void remove_by_name(char[]);
    void display(short);
    void display_top5();
    void Update(book,int);
    int rec_length(short);
    int search_By_id(int);
    int search_By_name(char[]);
};
#endif