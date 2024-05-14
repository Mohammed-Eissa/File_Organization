#include "book.h"
using namespace std;

int main()
{
    book ch;
    Book b;
    int t;
    while (true)
    {
        cout << "enter choice";
        cin >> t;
        if (t == 1)
        {
            cout << "enter your info " << endl;
            cout << "Enter book ID : ";
            cin >> b.id;
            cin.ignore();
            cout << "Enter book Name : ";
            cin.getline(b.name, 20);
            cout << "Enter book Category : ";
            cin.getline(b.category, 20);
            cout << "Enter Author Name : ";
            cin.getline(b.author, 20);
            ch.insert(b);
        }
        else if (t == 2)
        {
            ch.display_top5();
        }
        else if (t == 3)
        {
            cout << "enter id";
            int id;
            char name[20];
            // cin.ignore();
            // cin.getline(name,20);
            cin >> id;
            ch.remove_by_id(id);
        }
        else if (t == 4)
        {
            ch.printv();
        }
        else
            break;
    }
}
