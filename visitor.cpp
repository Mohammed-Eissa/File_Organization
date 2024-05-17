#include "visitor.h"
const char delimiter = '|';
const char deletedFlag = '*';
Visitor::Visitor()
{
    initial();
    tmp_id = -1;
    char c[50];
    ifstream visitor("visitor.txt", ios::binary);
    visitor.seekg(0, ios::beg);
    visitor.read((char *)&header, sizeof(short));
    visitor.close();
    ifstream prim("visitor_primary.txt", ios::binary);
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

    ifstream sec("visitor_secondry.txt", ios::binary);
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
        secondry[string(name)].insert(id);
    }
    sec.close();
}

Visitor::~Visitor()
{
    fstream prim("visitor_primary.txt", ios::out | ios::binary);
    for (auto [i, j] : primary)
    {
        prim.write((char *)&i, sizeof(int));
        prim.write((char *)&j, sizeof(int));
    }
    primary.clear();
    prim.close();
    fstream sec("visitor_secondry.txt", ios::out | ios::binary);
    char name[50];
    string tmp;
    int len;
    for (auto [i, j] : secondry)
    {
        len = sizeof(i);
        for (int k = 0; k < len; k++)
            name[k] = i[k];
        name[len] = '\0';
        for (auto k : j)
            sec.write((char *)&name, 50), sec.write((char *)&k, sizeof(int));
    }
    sec.close();
    fstream visitor("visitor.txt", ios::in | ios::out | ios::binary);
    visitor.seekp(0, ios::beg);
    visitor.write((char *)&header, sizeof(short));
    visitor.close();
}

void Visitor::initial()
{
    fstream visitor("visitor.txt", ios::in | ios::out | ios::binary);
    visitor.seekg(0, ios::end);
    if (visitor.tellg() == 0)
    {
        header = -1;
        visitor.write((char *)&header, sizeof(short));
    }
}

void Visitor::insert(visitor v)
{
    int id_len = sizeof(v.id);
    int name_len = strlen(v.name) + 1;
    int date_len = strlen(v.Visit_Date) + 1;
    int book_len = strlen(v.Borrowed_book) + 1;
    int total_len = id_len + name_len + date_len + book_len + 4 * sizeof(int);
    if (header == -1)
        insert_end(v);
    else
    {
        if (!insert_available(v, total_len))
            insert_end(v);
    }
}

bool Visitor::insert_available(visitor v, int t_len)
{
    ifstream visitorr("visitor.txt", ios::binary);
    fstream visitorw("visitor.txt", ios::in | ios::out | ios::binary);
    // cout<<t_len<<'\n';
    // if(visitorr.fail()||visitorw.fail()){cout<<"5555555555555\n";return false;}
    short next = header, cur, prev = 0;
    while (true)
    {
        int len;
        cur = next;
        visitorr.seekg(next + 1, ios::beg);
        visitorr.read((char *)&next, sizeof(short));
        visitorr.read((char *)&len, sizeof(int));
        int x;
        // cout<<prev<<' '<<cur<<' '<<next<<' '<<len<<'\n';
        // cin>>x;
        if (len >= t_len)
        {
            visitorw.seekp(cur, ios::beg);
            int id_len = sizeof(v.id);
            int name_len = strlen(v.name) + 1;
            int date_len = strlen(v.Visit_Date) + 1;
            int book_len = strlen(v.Borrowed_book) + 1;
            visitorw.write((char *)&id_len, sizeof(int));
            visitorw.write((char *)&v.id, id_len);
            visitorw.write((char *)&name_len, sizeof(int));
            visitorw.write(v.name, name_len);
            visitorw.write((char *)&date_len, sizeof(int));
            visitorw.write(v.Visit_Date, date_len);
            visitorw.write((char *)&book_len, sizeof(int));
            visitorw.write(v.Borrowed_book, book_len);
            tmp_id = v.id;
            tmp_name = string(v.name);
            primary[tmp_id] = cur;
            secondry[tmp_name].insert(tmp_id);
            cout << "visitor added successfully.\n";
            visitorr.seekg(visitorw.tellp(), ios::beg);
            char c, r = '_';
            visitorr.get(c);
            while (c != '|')
            {
                visitorw.write((char *)&r, sizeof(char));
                visitorr.get(c);
            }
            visitorw.seekp(prev, ios::beg);
            if (visitorw.tellp() == 0 || prev == -1)
                header = next;
            else
            {
                visitorw.seekp(1, ios::cur);
                visitorw.write((char *)&next, sizeof(short));
            }
            visitorr.close();
            visitorw.close();
            return true;
        }
        if (next == -1)
            return false;
        prev = cur;
    }
}

void Visitor::insert_end(visitor v)
{
    ofstream visitor("visitor.txt", ios::binary | ios::out | ios::app);

    if (visitor.is_open())
    {
        int id_len = sizeof(v.id);
        int name_len = strlen(v.name) + 1;
        int date_len = strlen(v.Visit_Date) + 1;
        int book_len = strlen(v.Borrowed_book) + 1;

        visitor.seekp(0, ios::end);
        short offset = visitor.tellp();
        visitor.write((char *)&id_len, sizeof(int));
        visitor.write((char *)&v.id, id_len);
        visitor.write((char *)&name_len, sizeof(int));
        visitor.write(v.name, name_len);
        visitor.write((char *)&date_len, sizeof(int));
        visitor.write(v.Visit_Date, date_len);
        visitor.write((char *)&book_len, sizeof(int));
        visitor.write(v.Borrowed_book, book_len);
        visitor.put(delimiter);
        tmp_id = v.id;
        tmp_name = string(v.name);
        primary[tmp_id] = offset;
        secondry[tmp_name].insert(tmp_id);
        cout << "visitor added successfully.\n";
    }
    else
        cout << "Can't open visitor file\n";

    visitor.close();
}

void Visitor::remove_by_id(int id)
{
    short offset = search_By_id(id);
    if (offset == -1)
    {
        cout << "There is no visitor with this ID\n";
        return;
    }
    int len = rec_length(offset);
    fstream visitor("visitor.txt", ios::in | ios::out | ios::binary);
    visitor.seekp(offset, ios::beg);
    visitor.write((char *)&deletedFlag, sizeof(char));
    visitor.write((char *)&header, sizeof(short));
    visitor.write((char *)&len, sizeof(int));
    header = offset;
    visitor.close();
    primary.erase(tmp_id);
    secondry[tmp_name].erase(tmp_id);
    if (secondry[tmp_name].empty())
        secondry.erase(tmp_name);
    cout << "visitor deleted successfully\n";
}
void Visitor::remove_by_name(char name[])
{
    short offset = search_By_name(name,1);
    if (offset == -1)
    {
        cout << "There is no visitor with this ID\n";
        return;
    }
    int len = rec_length(offset);
    fstream visitor("visitor.txt", ios::in | ios::out | ios::binary);
    visitor.seekp(offset, ios::beg);
    visitor.write((char *)&deletedFlag, sizeof(char));
    visitor.write((char *)&header, sizeof(short));
    visitor.write((char *)&len, sizeof(len));
    header = offset;
    visitor.close();
    primary.erase(tmp_id);
    secondry[tmp_name].erase(tmp_id);
    if (secondry[tmp_name].empty())
        secondry.erase(tmp_name);
    cout << "visitor deleted successfully\n";
}

int Visitor::search_By_id(int id)
{
    visitor student;
    int len;
    if (primary.find(id) != primary.end())
    {
        tmp_id = id;
        ifstream visitor("visitor.txt", ios::binary);
        visitor.seekg(primary[id]);
        visitor.read((char *)&len, sizeof(int));
        visitor.read((char *)&student.id, len);
        visitor.read((char *)&len, sizeof(int));
        visitor.read(student.name, len);
        visitor.read((char *)&len, sizeof(int));
        visitor.read((char *)&student.Visit_Date, len);
        visitor.read((char *)&len, sizeof(int));
        visitor.read((char *)&student.Borrowed_book, len);
        tmp_name = student.name;
        visitor.close();
        return primary[id];
    }
    return -1;
}

int Visitor::search_By_name(char name[50], bool flag)
{
    string t(name);
    int n = t.size();
    vector<pair<string, int>> idx;
    for (auto [i, j] : secondry)
    {
        if (n <= i.size())
        {
            if (i.substr(0, n) == t)
                for (auto k : j)
                    idx.emplace_back(i, k);
        }
    }
    if (!idx.size())
        return -1;
    if (idx.size() == 1)
    {
        tmp_name = idx[0].first;
        tmp_id = idx[0].second;
        return primary[tmp_id];
    }
    else
    {
        int j = 1;
        cout << "There is multible visitors with the same name : \n";
        for (auto i : idx)
        {
            cout << "Visitor " << j++ << ":- ";
            display(primary[i.second]);
        }
        if (flag)
        {
            cout << "Please chose the number of the visitor from previous visitors data : ";
            int number;
            cin >> number;
            tmp_name = idx[number - 1].first;
            tmp_id = idx[number - 1].second;
            return primary[tmp_id];
        }
        return -2;
    }
}

void Visitor::display(short off)
{
    if (off == -1)
    {
        cout << "Visitor not found\n";
        return;
    }
    if (off == -2)
        return;
    ifstream visitors("visitor.txt", ios::binary);
    visitors.seekg(off, ios::beg);
    visitor student;

    int len;
    visitors.read((char *)&len, sizeof(int));
    visitors.read((char *)&student.id, len);
    visitors.read((char *)&len, sizeof(int));
    visitors.read((char *)&student.name, len);
    visitors.read((char *)&len, sizeof(int));
    visitors.read((char *)&student.Visit_Date, len);
    visitors.read((char *)&len, sizeof(int));
    visitors.read((char *)&student.Borrowed_book, len);
    cout << "ID: " << student.id << ", Name: " << student.name << ", author: " << student.Visit_Date << ", category: " << student.Borrowed_book << endl;
    char delim;
    visitors.get(delim);
    while (delim == '_')
        visitors.get(delim);
}

void Visitor::display_top5()
{
    ifstream visitors("visitor.txt", ios::binary);
    visitors.seekg(0, ios::end);
    int end_of_file = visitors.tellg();
    visitors.seekg(2, ios::beg);
    visitor student;
    int i = 0;
    while (visitors.tellg() < end_of_file && i < 5)
    {
        char firstChar;
        visitors.read((char *)&firstChar, sizeof(char));
        if (firstChar == '*')
        {
            char ch;
            while (true)
            {
                visitors.get(ch);
                if (ch == '|')
                {
                    break;
                }
            }
            continue;
        }
        visitors.seekg(-1, ios::cur);
        int len;
        visitors.read((char *)&len, sizeof(int));
        visitors.read((char *)&student.id, len);
        visitors.read((char *)&len, sizeof(int));
        visitors.read(student.name, len);
        visitors.read((char *)&len, sizeof(int));
        visitors.read((char *)&student.Visit_Date, len);
        visitors.read((char *)&len, sizeof(int));
        visitors.read((char *)&student.Borrowed_book, len);
        cout << "ID: " << student.id << ", Name: " << student.name << ", Visit Date: " << student.Visit_Date << ", Borrowed book: " << student.Borrowed_book << endl;
        char delim;
        visitors.get(delim);
        while (delim == '_')
            visitors.get(delim);
        i++;
    }
    if (!i)
        cout << "No Visitors To Display\n";
}

void Visitor::Update(visitor v, int id)
{

    short off = search_By_id(id);
    if (off == -1)
        return void(cout << "There is no visitors with this id\n");
    int len_old = rec_length(off);
    int id_len = sizeof(v.id);
    int name_len = strlen(v.name) + 1;
    int book_len = strlen(v.Borrowed_book) + 1;
    int date_len = strlen(v.Visit_Date) + 1;
    int total_len = id_len + name_len + book_len + date_len + 4 * sizeof(int);
    ifstream visitorr("visitor.txt", ios::binary);
    fstream visitorw("visitor.txt", ios::in | ios::out | ios::binary);
    if (len_old >= total_len)
    {
        visitorw.seekp(off, ios::beg);

        visitorw.write((char *)&id_len, sizeof(int));
        visitorw.write((char *)&v.id, id_len);

        visitorw.write((char *)&name_len, sizeof(int));
        visitorw.write((char *)&v.name, name_len);

        visitorw.write((char *)&date_len, sizeof(int));
        visitorw.write((char *)&v.Visit_Date, date_len);

        visitorw.write((char *)&book_len, sizeof(int));
        visitorw.write((char *)&v.Borrowed_book, book_len);

        secondry[tmp_name].erase(tmp_id);
        if (secondry[tmp_name].empty())
            secondry.erase(tmp_name);

        tmp_id = id;
        tmp_name = string(v.name);
        primary[tmp_id] = off;
        secondry[tmp_name].insert(tmp_id);
        cout << "visitor Updated successfully.\n";
        visitorr.seekg(visitorw.tellp(), ios::beg);

        char ch;
        visitorr.get(ch);
        while (ch != '|')
        {
            visitorw.put('_');
            visitorr.get(ch);
        }
        visitorr.close();
        visitorw.close();
    }
    else
    {
        visitorr.close();
        visitorw.close();
        remove_by_id(id);
        insert(v);
        cout << "visitor Updated successfully.\n";
    }
}

void Visitor::Update(visitor &v, char name[])
{
    short off = search_By_name(name,1);
    if (off == -1)
        return void(cout << "There is no visitors with this id\n");
    cout << "Enter New Visit Date : ";
    cin.getline(v.Visit_Date, 50);
    cout << "Enter New Borrowed book : ";
    cin.getline(v.Borrowed_book, 50);
    int sz = tmp_name.size();
    for (int i = 0; i < sz; i++)
        v.name[i] = tmp_name[i];
    v.name[sz] = '\0';
    v.id = tmp_id;
    int len_old = rec_length(off);
    int id_len = sizeof(v.id);
    int name_len = strlen(v.name) + 1;
    int book_len = strlen(v.Borrowed_book) + 1;
    int date_len = strlen(v.Visit_Date) + 1;
    int total_len = id_len + name_len + book_len + date_len + 4 * sizeof(int);
    ifstream visitorr("visitor.txt", ios::binary);
    fstream visitorw("visitor.txt", ios::in | ios::out | ios::binary);
    if (len_old >= total_len)
    {
        visitorw.seekp(off, ios::beg);

        visitorw.write((char *)&id_len, sizeof(int));
        visitorw.write((char *)&v.id, id_len);

        visitorw.write((char *)&name_len, sizeof(int));
        visitorw.write((char *)&v.name, name_len);

        visitorw.write((char *)&date_len, sizeof(int));
        visitorw.write((char *)&v.Visit_Date, date_len);

        visitorw.write((char *)&book_len, sizeof(int));
        visitorw.write((char *)&v.Borrowed_book, book_len);

        tmp_id = v.id;
        tmp_name = string(v.name);
        primary[tmp_id] = off;
        secondry[tmp_name].insert(tmp_id);
        cout << "visitor Updated successfully.\n";
        visitorr.seekg(visitorw.tellp(), ios::beg);

        char ch;
        visitorr.get(ch);
        while (ch != '|')
        {
            visitorw.put('_');
            visitorr.get(ch);
        }
        visitorr.close();
        visitorw.close();
    }
    else
    {
        visitorr.close();
        visitorw.close();
        remove_by_id(v.id);
        insert(v);
        cout << "visitor Updated successfully.\n";
    }
}

int Visitor::rec_length(short offset)
{
    ifstream visitor("visitor.txt", ios::binary);
    visitor.seekg(offset, ios::beg);
    char c;
    int len = 0;
    visitor.get(c);
    while (c != '|')
        visitor.get(c), len++;
    visitor.close();
    return len;
}