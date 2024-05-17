## Project Description

The project is a simple file-based system for managing library records in C++. The system stores book records and visitor records using a variable-length-field and delimiter record approach in the following order:

### First : book records order:
> Id_Leangth Id Name_length Name Author_length Author Category_length Category|...

### Second : visitor records order:
> Id_Leangth Id Name_length Name Visit_Date_length Visit_Date Borrowed_Book_length Borrowed_Book|...

The Spaces is just for easy read, no spaces written in record.


## Book Information

The `Book` class contains the following fields:

- `Id_length` is an integer representing the book's ID length
- `Id` is an integer representing the book's ID
- `Name_length` is an integer representing the book's name length
- `Name` is a string representing the book's name
- `Author_length` is an integer representing the book's author length
- `Author` is a string representing the book's author
- `Category_length` is an integer representing the book's Category length
- `Category` is a string representing the book's Category

## Visitor Information

The `Visitor` class contains the following fields:

- `Id_length` is an integer representing the visitor's ID length
- `Id` is an integer representing the visitor's ID
- `Name_length` is an integer representing the visitor's name length
- `Name` is a string representing the visitor's name
- `Visit_date_length` is an integer representing the visitor's visit date length
- `Visit_date` is a string representing the visitor's visit date
- `Borrowed_book_length` is an integer representing the book's borrowed book length
- `Borrowed_book` is a string representing the book's borrowed book

The project provides several operations for managing book and visitor records, including adding a new record, updating an existing record by ID or name, deleting a record by ID or name, searching for records by ID or name, and displaying top 5 records.