# Library Management System

This is a Program written in C++ that manages the working of a library. The system supports three types of users: *Librarians, Students, and Faculty*. Students and Faculty can borrow, return, and view books, while Librarians can manage books and users.

### Compilation & Execution Instructions

#### Compilation Command
 bash
g++ cs253_assignment.cpp -o cs253_assignment  

#### Execution Command
bash
.\cs253_assignment

### Features

1. **User Roles**:
   - **Students**: Can borrow up to 3 books for 15 days. Fines are applied for overdue books.
   - **Faculty**: Can borrow up to 5 books for 30 days. No fines are applied for overdue books.
   - **Librarians**: Can manage books and users (add, remove, update).

2. **Book Management**:
   - Add new books to the library.
   - Remove existing books from the library.
   - Update book details such as title, author, publisher, year, ISBN, and status.

3. **Borrowing and Returning Books**:
   - Users can borrow available books.
   - Books can be returned with automatic status updates.

4. **Fine Management**:
   - Automatically calculates fines for overdue books (for students only).
   - Students can view and pay their fines.

5. **Data Persistence**:
   - Save all changes (books and user data) to files (`books.csv` and `borrowed_books.csv` and `borrowing_history.csv` and `fines.csv` and `users.csv` ) for persistent storage.

### Requirements

#### Software Requirements
- A C++ compiler like g++ (part of GCC).
- Visual Studio Code (or any text editor/IDE).

#### Files Required
- `books.csv`: Stores the list of the books in the library.
- `users.csv`: Stores the list of registered users.
- `borrowed_books.csv`:stores the  list of books borrowed by the user.
- `borrowing_history.csv`:stores the history of books borrowed by the user.
- `fines.csv`:store the fine amount of students.
- `users.csv`: stores the list of users.
 
Ensure these files exist in the project directory. If they do not exist, the program will create empty files during execution.

### How to Use

1. When you run the program, you will see a menu like this:

```
Library Management System
1. Login
2. Exit
Enter your choice:
select the number :  1 for login and 2 for Exit.
```

2. Select your role by entering the corresponding number.

#### For Students:
- Log in with your user ID and password available form users.csv file.
- After logging in, you can perform these actions:

--- STUDENT DASHBOARD (Student 1) ---
1. View Available Books
2. Borrow a Book
3. Return a Book
4. View Borrowed Books
5. Check Fine Amount
6. Pay Fine
7. Reserve a Book
8. View Borrowing History
9. View Overdue Books and Days
10. Logout
Enter your choice: 
select the choices to see different functionalities

#### For Faculty:
- Log in with your user ID and password.
- After logging in, you can perform these actions:

--- FACULTY DASHBOARD (Faculty 2) ---
1. View Available Books
2. Borrow a Book
3. Return a Book
4. View Borrowed Books
5. Reserve a Book
6. View Borrowing History
7. View Overdue Books and Days
8. Logout
Enter your choice:

#### For Librarians:
- Log in with your user ID and password.
- After logging in, you can perform these actions:
 --- LIBRARIAN DASHBOARD (Librarian) ---
1. View Books
2. Add Book
3. Remove Book
4. Update Book
5. Add User
6. Remove User
7. Logout
Enter your choice:

### Sample Data

To get started, you can use the following sample data for `books.csv` and `users.csv`.

#### Sample books.csv

```csv
Title,Author,Publisher,Year,ISBN,Status,ReservedBy,BorrowedDate
"1984","George Orwell","Secker and Warburg",1949,9780451524935,Available,,1741625320
"To Kill a Mockingbird","Harper Lee","JB Lippincott and Co",1960,9780061120084,Available,,
"The Great Gatsby","FScott Fitzgerald","Charles Scribners Sons",1925,9780743273565,Available,,
"Moby Dick","Herman Melville","Richard Bentley",1851,9780143052553,Available,,
"War and Peace","Leo Tolstoy","The Russian Messenger",1869,9780140444179,Available,,
"Hamlet","William Shakespeare","unknown",1603,9780743477123,Available,,
"The Odyssey","Homer","unknown",-800,9780143039950,Available,,
"The Divine Comedy","Dante Alighieri","unknown",1320,9780140441000,Available,,
"One Hundred Years of Solitude","Gabriel Garcia Marquez","Harper and Row",1967,9780061120039,Available,,
"The Brothers Karamazov","Fyodor Dostoevsky","The Russian Messenger",1880,9780374528375,Available,,
"The Catcher in the Rye","JD Salinger","Little Brown and Company",1951,9780316769532,Available,,
"Crime and Punishment","Fyodor Dostoevsky","The Russian Messenger",1866,9780307950654,Available,,
"Alices Adventures in Wonderland","Lewis Carroll","Macmillan",1865,9780141321004,Available,,
"The Adventures of Sherlock Holmes","Arthur Conan Doyle","George Newnes Ltd",1892,9780192833558,Available,,
"Don Quixote","Miguel de Cervantes","Francisco de Robles",1605,9780060934347,Available,,
"Ulysses","James Joyce","Shakespeare and Company",1922,9781847492754,Available,,
"The Picture of Dorian Gray","Oscar Wilde","Ward Lock and Company",1890,9780141439570,Available,,
"Anna Karenina","Leo Tolstoy","The Russian Messenger",1877,9780143035006,Available,,

```

#### Sample users.csv

```csv
Name,ID,Password,Role
Faculty 2,F2,password,Faculty
Faculty 3,F3,password,Faculty
Faculty 4,F4,password,Faculty
Faculty 5,F5,password,Faculty
Faculty 6,F6,password,Faculty
Faculty 7,F7,password,Faculty
Faculty 8,F8,password,Faculty
Faculty 9,F9,password,Faculty
Librarian,L1,password,Librarian
Student 1,S1,password,Student
Student 10,S10,password,Student
Student 2,S2,password,Student
Student 3,S3,password,Student
Student 4,S4,password,Student
Student 5,S5,password,Student
Student 6,S6,password,Student
Student 7,S7,password,Student
Student 8,S8,password,Student
Student 9,S9,password,Student

```
With this setup, you can start using the Library Management System immediately.
