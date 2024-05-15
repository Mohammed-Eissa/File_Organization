#include "book.h"
const char delimiter = '|';
const char deletedFlag = '*';
Book::Book()
{
    initial();
    tmp_id = -1;
    char c[50];
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
    char name[50];
    while (true)
    {
        if (sec.tellg() == is_end)
            break;
        sec.read((char *)&name, 50);
        sec.read((char *)&id, sizeof(int));
        string s(name);
        secondry[string(name)] = id;
    }
    sec.close();
}

Book::~Book()
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
    char name[50];
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
        sec.write((char *)&name, 50);
        sec.write((char *)&j, sizeof(int));
    }
    sec.close();
    fstream books("book.txt", ios::in | ios::out | ios::binary);
    books.seekp(0, ios::beg);
    books.write((char *)&header, sizeof(short));
    books.close();
}

void Book::initial()
{
    fstream books("book.txt", ios::in | ios::out | ios::binary);
    books.seekg(0, ios::end);
    if (books.tellg() == 0)
    {
        header = -1;
        books.write((char *)&header, sizeof(short));
    }
}

void Book::insert(book b)
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

bool Book::insert_available(book b, int t_len)
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

void Book::insert_end(book b)
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

void Book::remove_by_id(int id)
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
    cout << header << ' ' << len << '\n';
    header = offset;
    books.close();
    primary.erase(tmp_id);
    secondry.erase(tmp_name);
    cout << "Book deleted successfully\n";
}
void Book::remove_by_name(char name[])
{
    short offset = search_By_name(name);
    if (offset == -1)
    {
        cout << "There is no books with this Name\n";
        return;
    }
    fstream books("book.txt", ios::in | ios::out | ios::binary);
    books.seekp(offset, ios::beg);
    books.write((char *)&deletedFlag, sizeof(char));
    books.write((char *)&header, sizeof(short));
    header = offset;
    books.close();
    primary.erase(tmp_id);
    secondry.erase(tmp_name);
    cout << "Book deleted successfully\n";
}

int Book::search_By_id(int id)
{
    book b;
    int len;
    if (primary.find(id) != primary.end())
    {
        tmp_id = id;
        ifstream books("book.txt", ios::binary);
        books.seekg(primary[id]);
        books.read((char *)&len, sizeof(int));
        books.read((char *)&b.id, len);
        books.read((char *)&len, sizeof(int));
        books.read(b.name, len);
        books.read((char *)&len, sizeof(int));
        books.read((char *)&b.author, len);
        books.read((char *)&len, sizeof(int));
        books.read((char *)&b.category, len);
        tmp_name = b.name;
        books.close();
        return primary[id];
    }
    return -1;
}

int Book::search_By_name(char name[50])
{
    string t(name);
    int n = t.size();
    vector<string> idx;
    for (auto [i, j] : secondry)
    {
        if (n <= i.size())
        {
            if (i.substr(0, n) == t)
                idx.push_back(i);
        }
    }
    if (!idx.size())
        return -1;
    if (idx.size() == 1)
    {
        tmp_name = idx[0];
        tmp_id = secondry[tmp_name];
        return primary[tmp_id];
    }
    else
    {
        int j = 1;
        cout << "There is multible Books with the same name : \n";
        for (auto i : idx)
        {
            cout << "Book " << j++ << ":- ";
            display(primary[secondry[i]]);
        }
        cout << "Please chose the number of the Book from previous Books data : ";
        int number;
        cin >> number;
        tmp_name = idx[number - 1];
        tmp_id = secondry[tmp_name];
        return primary[tmp_id];
    }
}

void Book::display(short off)
{
    if (off == -1)
    {
        cout << "Book not found\n";
        return;
    }

    ifstream books("book.txt", ios::binary);
    books.seekg(off, ios::beg);
    book b;

    int len;
    books.read((char *)&len, sizeof(int));
    books.read((char *)&b.id, len);
    books.read((char *)&len, sizeof(int));
    books.read((char *)&b.name, len);
    books.read((char *)&len, sizeof(int));
    books.read((char *)&b.author, len);
    books.read((char *)&len, sizeof(int));
    books.read((char *)&b.category, len);
    cout << "ID: " << b.id << ", Name: " << b.name << ", author: " << b.author << ", category: " << b.category << endl;
    char delim;
    books.get(delim);
    while (delim == '_')
        books.get(delim);
}

void Book::display_top5()
{
    ifstream books("book.txt", ios::binary);
    books.seekg(0, ios::end);
    int end_of_file = books.tellg();
    books.seekg(2, ios::beg);
    int c = 0;
    book b;
    int i = 0;
    while (books.tellg() < end_of_file && i < 5)
    {
        char firstChar;
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
        books.read((char *)&b.id, len);
        books.read((char *)&len, sizeof(int));
        books.read(b.name, len);
        books.read((char *)&len, sizeof(int));
        books.read((char *)&b.author, len);
        books.read((char *)&len, sizeof(int));
        books.read((char *)&b.category, len);
        cout << "ID: " << b.id << ", Name: " << b.name << ", author: " << b.author << ", category: " << b.category << endl;
        char delim;
        books.get(delim);
        while (delim == '_')
            books.get(delim);
        i++;
    }
    if(!i)cout<<"No Books To Display\n";
}

void Book::Update(book b, int id)
{
    short off = search_By_id(id);
    if (off == -1)
        return void(cout << "There is no books with this id\n");
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
        bookw.close();
    }
    else
    {
        bookr.close();
        bookw.close();
        remove_by_id(id);
        insert(b);
        cout << "Book Updated successfully.\n";
    }
}

void Book::Update(book &b, char name[])
{
    short off = search_By_name(name);
    if (off == -1)
        return void(cout << "There is no books with this name\n");
    cin.ignore();
    cout << "Enter New book Category : ";
    cin.getline(b.category, 50);
    cout << "Enter New Author Name : ";
    cin.getline(b.author, 50);
    int sz = tmp_name.size();
    for (int i = 0; i < sz; i++)
        b.name[i] = tmp_name[i];
    b.name[sz] = '\0';
    b.id = tmp_id;
    int len_old = rec_length(off);
    int id_len = sizeof(b.id);
    int name_len = strlen(b.name) + 1;
    int author_len = strlen(b.author) + 1;
    int category_len = strlen(b.category) + 1;
    int total_len = id_len + name_len + author_len + category_len + 4 * sizeof(int);
    ifstream bookr("book.txt", ios::binary);
    fstream bookw("book.txt", ios::in | ios::out | ios::binary);
    if (len_old >= total_len)
    {
        bookw.seekp(off, ios::beg);

        bookw.write((char *)&id_len, sizeof(int));
        bookw.write((char *)&b.id, id_len);

        bookw.write((char *)&name_len, sizeof(int));
        bookw.write((char *)&b.name, name_len);

        bookw.write((char *)&author_len, sizeof(int));
        bookw.write((char *)&b.author, author_len);

        bookw.write((char *)&category_len, sizeof(int));
        bookw.write((char *)&b.category, category_len);

        tmp_id = b.id;
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
        bookw.close();
    }
    else
    {
        bookr.close();
        bookw.close();
        remove_by_id(b.id);
        insert(b);
        cout << "Book Updated successfully.\n";
    }
}

int Book::rec_length(short offset)
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