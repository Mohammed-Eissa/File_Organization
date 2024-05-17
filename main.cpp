#include "book.h"
#include "visitor.h"

void Main_Menu()
{
    Visitor V;
    visitor v;
    Book B;
    book b;
    //    Visitor v;           class of Visitor  +
    int choice;
    int ch;
    back:
    cout << "\nMain Menu:\n1. Work on Library Visitors\n2. Work on Library Books\n3. End the program\n";
    cin >> choice;
    "\n";
    switch (choice)
    {
    case 1:
    BackStep1:
        while (true)
        {
            cout << "\nMenu:\n1. Add A Visitor Data\n2. Display A Visitor Data\n3. Delete A Visitor Data\n4. Update A Visitor Data\n5. Find A Visitor\n6. Return to the Main Menu\n7. End the program\n";
            cin >> choice;
            switch (choice)
            {
            case 1:
                cout << "Adding Visitor :" << endl;
                cout << "Enter Visitor ID : ";
                cin >> v.id;
                cin.ignore();
                cout << "Enter Visitor Name : ";
                cin.getline(v.name, 50);
                cout << "Enter Visit_Date : ";
                cin.getline(v.Visit_Date, 50);
                cout << "Enter Borrowed_book : ";
                cin.getline(v.Borrowed_book, 50);
                while(V.search_By_id(v.id)!=-1)
                {
                    cout << "This ID already exist please choose another one : ";
                    cin >> v.id;
                }
                V.insert(v); ///+
                break;
            case 2:
                cout << "\n1. Display a specific Visitor by ID \n2. Display a specific Visitor by name \n3. Display all Visitors\n4. Go back \n"
                     << endl;
                cin >> ch;
                switch (ch)
                {
                case 1:
                    cout << "Enter The ID of Visitor you want to display: ";
                    int searchID;
                    cin >> searchID;
                    V.display(V.search_By_id(searchID)); ///+
                    break;
                case 2:
                    cout << "Enter The Name of Visitor you want to display: ";
                    char searchN[50];
                    cin.ignore();
                    cin.getline(searchN,50);
                    V.display(V.search_By_name(searchN)); ///+
                    break;
                case 3:
                    V.display_top5(); ///+
                    break;
                case 4:
                    goto BackStep1;
                    break;
                default:
                    cout << "Invalid choice" << endl;
                }
                break;
            case 3:
                cout << "\n1. Delete a specific Visitor By ID \n2. Delete a specific Visitor By Name \n3. Go back \n"
                     << endl;
                cin >> ch;
                switch (ch)
                {
                case 1:
                    cout << "Enter ID of Visitor you want to delete: ";
                    int deleteId;
                    cin >> deleteId;
                    V.remove_by_id(deleteId); ///+
                    break;
                case 2:
                    cout << "Enter Name of Visitor you want to delete: ";
                    char deleteN[50];
                    cin.ignore();
                    cin.getline(deleteN,50);
                    V.remove_by_name(deleteN); ///+
                    break;
                case 3:
                    goto BackStep1;
                    break;
                default:
                    cout << "Invalid choice" << endl;
                }
                break;
            case 4:
                cout << "\n1. Update a specific Visitor by ID \n2. Update a specific Visitor by name \n3. Go back \n"
                     << endl;
                cin >> ch;
                switch (ch)
                {
                case 1:
                    cout << "Enter The ID of Visitor you want to Update: ";
                    cin >> v.id;
                    cin.ignore();
                    cout << "Enter New Visitor Name : ";
                    cin.getline(v.name, 50);
                    cout << "Enter New Visit Date : ";
                    cin.getline(v.Visit_Date, 50);
                    cout << "Enter New Borrowed book : ";
                    cin.getline(v.Borrowed_book, 50);
                    V.Update(v, v.id); ///+
                    break;
                case 2:
                    cout << "Enter The Name of Visitor you want to Update: ";
                    char name[50];
                    cin.ignore();
                    cin.getline(name, 50);
                    V.Update(v,v.name); ///+
                    break;
                case 3:
                    goto BackStep1;
                    break;
                default:
                    cout << "Invalid choice" << endl;
                }
                break;
            case 5:
                cout << "\n1. Search By ID \n2. Search By Name \n3. Go back \n"
                     << endl;
                cin >> ch;
                switch (ch)
                {
                case 1:
                    cout << "Enter The ID of the Visitor you are searching for: ";
                    int searchID;
                    cin >> searchID;
                    V.display(V.search_By_id(searchID)); ///+
                    break;
                case 2:
                    cout << "Enter The Name of the Visitor you are searching for: ";
                    char searchN[50];
                    cin.ignore();
                    cin.getline(searchN,50);
                    V.display(V.search_By_name(searchN)); ///+
                    break;
                case 3:
                    goto BackStep1;
                    break;
                default:
                    cout << "Invalid choice" << endl;
                }
                break;
            case 6:
                goto back;
            case 7:
                return;
            default:
                cout << "Invalid choice" << endl;
            }
        }
        break;
    case 2:
    BackStep:
        while (true)
        {
            cout << "\nMenu:\n1. Add A BOOK\n2. Display BOOK\n3. Delete A BOOK\n4. Update A BOOK\n5. Find A BOOK\n6. Return to the Main Menu\n7. End the program\n";
            cin >> choice;
            switch (choice)
            {
            case 1:
                cout << "Adding BOOK :" << endl;
                cout << "Enter book ID : ";
                cin >> b.id;
                cin.ignore();
                cout << "Enter book Name : ";
                cin.getline(b.name, 50);
                cout << "Enter book Category : ";
                cin.getline(b.category, 50);
                cout << "Enter Author Name : ";
                cin.getline(b.author, 50);
                while(B.search_By_id(b.id)!=-1)
                {
                    cout<<"This ID already exist please choose another one : ";
                    cin>>b.id;
                }
                B.insert(b);
                break;
            case 2:
                cout << "\n1. Display a specific book by ID \n2. Display a specific book by name \n3. Display all books\n4. Go back \n"
                     << endl;
                cin >> ch;
                switch (ch)
                {
                case 1:
                    cout << "Enter The ID of the Book you want to display: ";
                    int searchID;
                    cin >> searchID;
                    B.display(B.search_By_id(searchID));
                    break;
                case 2:
                    cout << "Enter The Name of the Book you want to display: ";
                    char searchN[50];
                    cin.ignore();
                    cin.getline(searchN,50);
                    B.display(B.search_By_name(searchN));
                    break;
                case 3:
                    B.display_top5();
                    break;
                case 4:
                    goto BackStep;
                    break;
                default:
                    cout << "Invalid choice" << endl;
                }
                break;
            case 3:
                cout << "\n1. Delete By ID \n2. Delete By Name \n3. Go back \n"
                     << endl;
                cin >> ch;
                switch (ch)
                {
                case 1:
                    cout << "Enter ID of BOOK to delete: ";
                    int deleteId;
                    cin >> deleteId;
                    B.remove_by_id(deleteId);
                    break;
                case 2:
                    cout << "Enter Name of BOOK to delete: ";
                    char deleteN[50];
                    cin.ignore();
                    cin.getline(deleteN,50);
                    B.remove_by_name(deleteN);
                    break;
                case 3:
                    goto BackStep;
                    break;
                default:
                    cout << "Invalid choice" << endl;
                }
                break;
            case 4:
                cout << "\n1. Update a specific Book by ID \n2. Update a specific Book by name \n3. Go back \n"
                     << endl;
                cin >> ch;
                switch (ch)
                {
                case 1:
                    cout << "Enter The ID of Book you want to Update: ";
                    cin >> b.id;
                    cin.ignore();
                    cout << "Enter New book Name : ";
                    cin.getline(b.name, 50);
                    cout << "Enter New book Category : ";
                    cin.getline(b.category, 50);
                    cout << "Enter New Author Name : ";
                    cin.getline(b.author, 50);
                    B.Update(b, b.id); ///+
                    break;
                case 2:
                    cout << "Enter The Name of Book you want to Update: ";
                    char name[50];
                    cin.ignore();
                    cin.getline(name, 50);
                    B.Update(b, name); ///+
                    break;
                case 3:
                    goto BackStep1;
                    break;
                default:
                    cout << "Invalid choice" << endl;
                }
                break;
            case 5:
                cout << "\n1. Search By ID \n2. Search By Name \n3. Go back \n"
                     << endl;
                cin >> ch;
                switch (ch)
                {
                case 1:
                    cout << "Enter The ID of the Book you are searching for: ";
                    int searchID;
                    cin >> searchID;
                    B.display(B.search_By_id(searchID));
                    break;
                case 2:
                    cout << "Enter The Name of the Book you are searching for: ";
                    char searchN[50];
                    cin.ignore();
                    cin.getline(searchN,50);
                    B.display(B.search_By_name(searchN));
                    break;
                case 3:
                    goto BackStep;
                    break;
                default:
                    cout << "Invalid choice" << endl;
                }
                break;
            case 6:
                goto back;
            case 7:
                return;
            default:
                cout << "Invalid choice" << endl;
            }
        }
        break;
    case 3:
        return;
    default:
        cout << "Invalid choice" <<endl;
    }
}

int main()
{
    cout << "         .. Welcome To The Library System .. \n";
    Main_Menu();
    cout << "\n\n          .. Thanks For Using Our Program .. ";
}