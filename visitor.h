#ifndef VISITOR_H
#define VISITOR_H
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string.h>
#include <vector>
#include <map>
#include <set>
using namespace std;

struct visitor
{
    int id;
    char name[50], Visit_Date[50], Borrowed_book[50];
};

class Visitor
{
private:
    short header;
    map<int, int> primary;
    map<string, set<int>> secondry;
    int tmp_id;
    string tmp_name;

public:
    Visitor();
    ~Visitor();
    void initial();
    void insert(visitor);
    bool insert_available(visitor, int);
    void insert_end(visitor);
    void remove_by_id(int);
    void remove_by_name(char[]);
    void display(short);
    void display_top5();
    void Update(visitor, int);
    void Update(visitor&, char[]);
    int rec_length(short);
    int search_By_id(int);
    int search_By_name(char[],bool=0);
};
#endif