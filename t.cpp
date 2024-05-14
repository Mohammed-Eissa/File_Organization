#include <iostream>
#include <fstream>
#include <algorithm>
#include <string.h>
#include <vector>
#include <map>
using namespace std;

struct Book
{
    int id;
    char name[20], author[20], category[20];
};

const char delimiter = '|';
const char deletedFlag = '*';

class book
{
private:
    short header;
    map<int, int> primary;
    map<string, int> secondry;
    int tmp_id;
    string tmp_name;

public:
    book();
    ~book();
    void initial();
    void insert(Book);
    bool insert_available(Book, int);
    void insert_end(Book);
    void remove_by_id(int);
    void remove_by_name(char[]);
    void display_top5();
    void printv();
    void Update(Book, int);
    int rec_length(short);
    int search_By_id(int);
    int search_By_name(char[]);
};

book::book()
{
    initial();
    tmp_id = -1;
    char c[20];
    ifstream books("book.txt", ios::binary);
    books.seekg(0, ios::beg);
    books.read((char *)&header, sizeof(short));
    books.close();
    ifstream prim("primary.txt", ios::binary);
    prim.seekg(0, ios::end);
    int is_end;
    is_end = prim.tellg();
    prim.seekg(0, ios::beg);
    int id, offset;
    while (true)
    {
        if (prim.tellg() == is_end)
            break;
        prim.read((char *)&id, sizeof(int));
        prim.read((char *)&offset, sizeof(int));
        primary[id] = offset;
    }
    prim.close();

    ifstream sec("secondry.txt", ios::binary);
    sec.seekg(0, ios::end);
    is_end = sec.tellg();
    sec.seekg(0, ios::beg);
    char name[20];
    while (true)
    {
        if (sec.tellg() == is_end)
            break;
        sec.read((char *)&name, 20);
        sec.read((char *)&id, sizeof(int));
        string s(name);
        secondry[string(name)] = id;
    }
    sec.close();
}

book::~book()
{
    fstream prim("primary.txt", ios::out | ios::binary);
    for (auto [i, j] : primary)
    {
        if (j == -1)
            continue;
        prim.write((char *)&i, sizeof(int));
        prim.write((char *)&j, sizeof(int));
    }
    primary.clear();
    prim.close();
    fstream sec("secondry.txt", ios::out | ios::binary);
    char name[20];
    string tmp;
    int len;
    for (auto [i, j] : secondry)
    {
        if (j == -1)
            continue;
        len = sizeof(i);
        for (int k = 0; k < len; k++)
            name[k] = i[k];
        name[len] = '\0';
        sec.write((char *)&name, 20);
        sec.write((char *)&j, sizeof(int));
    }
    sec.close();
    fstream books("book.txt", ios::in | ios::out | ios::binary);
    books.seekp(0, ios::beg);
    books.write((char *)&header, sizeof(short));
    books.close();
}

void book::initial()
{
    fstream books("book.txt", ios::in | ios::out | ios::binary);
    books.seekg(0, ios::end);
    if (books.tellg() == 0)
    {
        header = -1;
        books.write((char *)&header, sizeof(short));
    }
    books.close();
}

void book::insert(Book b)
{
    int id_len = sizeof(b.id);
    int name_len = strlen(b.name) + 1;
    int category_len = strlen(b.category) + 1;
    int author_len = strlen(b.author) + 1;
    int total_len = id_len + name_len + category_len + author_len + 4 * sizeof(int);
    if (header == -1)
        insert_end(b);
    else
    {
        if (!insert_available(b, total_len))
            insert_end(b);
    }
}

bool book::insert_available(Book b, int t_len)
{
    ifstream bookr("book.txt", ios::binary);
    fstream bookw("book.txt", ios::in | ios::out | ios::binary);
    // cout<<t_len<<'\n';
    // if(bookr.fail()||bookw.fail()){cout<<"5555555555555\n";return false;}
    short next = header, cur, prev = 0;
    while (true)
    {
        int len;
        cur = next;
        bookr.seekg(next + 1, ios::beg);
        bookr.read((char *)&next, sizeof(short));
        bookr.read((char *)&len, sizeof(int));
        int x;
        // cout<<prev<<' '<<cur<<' '<<next<<' '<<len<<'\n';
        // cin>>x;
        if (len >= t_len)
        {
            bookw.seekp(cur, ios::beg);
            int id_len = sizeof(b.id);
            int name_len = strlen(b.name) + 1;
            int category_len = strlen(b.category) + 1;
            int author_len = strlen(b.author) + 1;
            bookw.write((char *)&id_len, sizeof(int));
            bookw.write((char *)&b.id, id_len);
            bookw.write((char *)&name_len, sizeof(int));
            bookw.write(b.name, name_len);
            bookw.write((char *)&author_len, sizeof(int));
            bookw.write(b.author, author_len);
            bookw.write((char *)&category_len, sizeof(int));
            bookw.write(b.category, category_len);
            tmp_id = b.id;
            tmp_name = string(b.name);
            primary[tmp_id] = cur;
            secondry[tmp_name] = tmp_id;
            cout << "Book added successfully.\n";
            bookr.seekg(bookw.tellp(), ios::beg);
            char c, r = '_';
            bookr.get(c);
            while (c != '|')
            {
                bookw.write((char *)&r, sizeof(char));
                bookr.get(c);
            }
            bookw.seekp(prev, ios::beg);
            if (bookw.tellp() == 0 || prev == -1)
                header = next;
            else
            {
                bookw.seekp(1, ios::cur);
                bookw.write((char *)&next, sizeof(short));
            }
            bookr.close();
            bookw.close();
            return true;
        }
        if (next == -1)
            return false;
        prev = cur;
    }
}

void book::insert_end(Book b)
{
    ofstream books("book.txt", ios::binary | ios::out | ios::app);

    if (books.is_open())
    {
        int id_len = sizeof(b.id);
        int name_len = strlen(b.name) + 1;
        int category_len = strlen(b.category) + 1;
        int author_len = strlen(b.author) + 1;
        int total_len = id_len + name_len + category_len + author_len + 4 * sizeof(int) + 1;

        books.seekp(0, ios::end);
        short offset = books.tellp();
        books.write((char *)&id_len, sizeof(int));
        books.write((char *)&b.id, id_len);
        books.write((char *)&name_len, sizeof(int));
        books.write(b.name, name_len);
        books.write((char *)&author_len, sizeof(int));
        books.write(b.author, author_len);
        books.write((char *)&category_len, sizeof(int));
        books.write(b.category, category_len);
        books.put(delimiter);
        tmp_id = b.id;
        tmp_name = string(b.name);
        primary[tmp_id] = offset;
        secondry[tmp_name] = tmp_id;
        cout << "Book added successfully.\n";
    }
    else
        cout << "Can't open Book file\n";

    books.close();
}

void book::remove_by_id(int id)
{
    short offset = search_By_id(id);
    if (offset == -1)
    {
        cout << "There is no books with this ID\n";
        return;
    }
    int len = rec_length(offset);
    fstream books("book.txt", ios::in | ios::out | ios::binary);
    books.seekp(offset, ios::beg);
    books.write((char *)&deletedFlag, sizeof(char));
    books.write((char *)&header, sizeof(short));
    books.write((char *)&len, sizeof(int));
    // cout<<header<<' '<<len<<'\n';
    header = offset;
    books.close();
    primary.erase(tmp_id);
    secondry.erase(tmp_name);
}
void book::remove_by_name(char name[])
{
    short offset = search_By_name(name);
    if (offset == -1)
    {
        cout << "There is no books with this ID\n";
        return;
    }
    int len = rec_length(offset);
    fstream books("book.txt", ios::in | ios::out | ios::binary);
    books.seekp(offset, ios::beg);
    books.write((char *)&deletedFlag, sizeof(char));
    books.write((char *)&header, sizeof(short));
    books.write((char *)&len, sizeof(int));
    header = offset;
    books.close();
    primary.erase(tmp_id);
    secondry.erase(tmp_name);
}

int book::search_By_id(int id)
{
    Book student;
    int len;
    if (primary.find(id) != primary.end())
    {
        tmp_id = id;
        ifstream books("book.txt", ios::binary);
        books.seekg(primary[id]);
        books.read((char *)&len, sizeof(int));
        books.read((char *)&student.id, len);
        books.read((char *)&len, sizeof(int));
        books.read(student.name, len);
        books.read((char *)&len, sizeof(int));
        books.read((char *)&student.author, len);
        books.read((char *)&len, sizeof(int));
        books.read((char *)&student.category, len);
        tmp_name = student.name;
        books.close();
        return primary[id];
    }
    return -1;
}

int book::search_By_name(char name[20])
{
    string t(name);
    if (secondry.find(t) != secondry.end())
    {
        tmp_name = t;
        tmp_id = secondry[t];
        return primary[secondry[t]];
    }
    return -1;
}

void book::display_top5()
{
    ifstream books("book.txt", ios::binary);
    books.seekg(0, ios::end);
    int end_of_file = books.tellg();
    books.seekg(2, ios::beg);
    Book student;
    while (true)
    {
        char firstChar;
        if (books.tellg() == end_of_file)
        {
            break;
        }
        books.read((char *)&firstChar, sizeof(char));
        if (firstChar == '*')
        {
            char ch;
            while (true)
            {
                books.get(ch);
                if (ch == '|')
                {
                    break;
                }
            }
            continue;
        }
        books.seekg(-1, ios::cur);
        int len;
        books.read((char *)&len, sizeof(int));
        books.read((char *)&student.id, len);
        books.read((char *)&len, sizeof(int));
        books.read(student.name, len);
        books.read((char *)&len, sizeof(int));
        books.read((char *)&student.author, len);
        books.read((char *)&len, sizeof(int));
        books.read((char *)&student.category, len);
        cout << "ID: " << student.id << ", Name: " << student.name << ", author: " << student.author << ", category: " << student.category << endl;
        char delim;
        books.get(delim);
        while (delim == '_')
            books.get(delim);
    }
    books.close();
}
int book::rec_length(short offset)
{
    ifstream books("book.txt", ios::binary);
    books.seekg(offset, ios::beg);
    char c;
    int len = 0;
    books.get(c);
    while (c != '|')
        books.get(c), len++;
    books.close();
    return len;
}

void book::Update(Book b, int id)
{
    short off = search_By_id(id);
    int len_old = rec_length(off);
    int id_len = sizeof(b.id);
    int name_len = strlen(b.name) + 1;
    int category_len = strlen(b.category) + 1;
    int author_len = strlen(b.author) + 1;
    int total_len = id_len + name_len + category_len + author_len + 4 * sizeof(int);
    ifstream bookr("book.txt", ios::binary);
    fstream bookw("book.txt", ios::in | ios::out | ios::binary);
    if (len_old >= total_len)
    {
        bookw.seekp(off, ios::beg);

        bookw.write((char *)&id_len, sizeof(int));
        bookw.write((char *)&b.id, id_len);

        bookw.write((char *)&name_len, sizeof(int));
        bookw.write((char *)b.name, name_len);

        bookw.write((char *)&author_len, sizeof(int));
        bookw.write((char *)b.author, author_len);

        bookw.write((char *)&category_len, sizeof(int));
        bookw.write((char *)b.category, category_len);

        tmp_id = id;
        tmp_name = string(b.name);
        primary[tmp_id] = off;
        secondry[tmp_name] = tmp_id;
        cout << "Book Updated successfully.\n";
        bookr.seekg(bookw.tellp(), ios::beg);

        char ch;
        bookr.get(ch);
        while (ch != '|')
        {
            bookw.put('_');
            bookr.get(ch);
        }
        bookr.close();
    }
    else
        cout << "NOT VALID \n";
    bookw.close();
}

void book::printv()
{
    for (auto [i, j] : primary)
        cout << i << ' ' << j << '\n';
    cout << "\n\n";
    for (auto [i, j] : secondry)
        cout << i << ' ' << j << '\n';
    cout << "\n\n";
}

int main()
{
    int choice;
    Book B;
    book b;
    while (true)
    {
        cout << "\nMenu:\n1. Add BOOK\n2. Display BOOK\n3. Delete BOOK\n4. Update BOOK\n5. Exit\n";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cout << "Adding BOOK :" << endl;
            cout << "Enter book ID : ";
            cin >> B.id;
            cin.ignore();
            cout << "Enter book Name : ";
            cin.getline(B.name, 20);
            cout << "Enter book Category : ";
            cin.getline(B.category, 20);
            cout << "Enter Author Name : ";
            cin.getline(B.author, 20);
            b.insert(B);
            break;
        case 2:
            b.display_top5();
            break;
        case 3:
            cout << "Enter ID of BOOK to delete: ";
            int deleteId;
            cin >> deleteId;
            b.remove_by_id(deleteId);
            break;
        case 4:
            cout << "Enter ID of BOOK to update: ";
            cin >> B.id;
            cin.ignore();
            cout << "Enter book Name : ";
            cin.getline(B.name, 20);
            cout << "Enter book Category : ";
            cin.getline(B.category, 20);
            cout << "Enter Author Name : ";
            cin.getline(B.author, 20);
            b.Update(B, B.id);
            break;
        case 5:
            return 0;
        default:
            cout << "Invalid choice" << endl;
            break;
        }
    }
}