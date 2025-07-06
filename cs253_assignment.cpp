#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <map>
#include <sstream>
#include <memory>
#include <algorithm>
#include <exception>
using namespace std;

string trim(const string& str) {
    auto start = str.begin();
    while (start != str.end() && isspace(*start)) {
        start++;
    }
    auto end = str.end();
    do {
        end--;
    } while (distance(start, end) > 0 && isspace(*end));
    return string(start, end + 1);
}

class Book {
public:
    string title;
    string author;
    string publisher;
    int year;
    string ISBN;
    string status;
    string reservedBy;
    time_t borrowedDate;

    Book() {}
    Book(string t, string a, string p, int y, string i)
        : title(t), author(a), publisher(p), year(y), ISBN(i), status("Available"), reservedBy(""), borrowedDate(0) {}

    void printDetails() const {
        cout << "Title: " << title << ", Author: " << author << ", Publisher: " << publisher
            << ", Year: " << year << ", ISBN: " << ISBN << ", Status: " << status;
        if (!reservedBy.empty()) {
            cout << ", Reserved By: " << reservedBy;
        }
        cout << endl;
    }
};

class Account {
public:
    vector<Book> borrowedBooks;
    vector<Book> borrowingHistory;
    int overdueFine;

    Account() : overdueFine(0) {}

    void addBorrowedBook(Book& book) {
        book.borrowedDate = time(0); 
        borrowedBooks.push_back(book);
    }

    void returnBorrowedBook(const string& ISBN, int maxBorrowingPeriod) {
        for (auto it = borrowedBooks.begin(); it != borrowedBooks.end(); ++it) {
            if (it->ISBN == ISBN) {
                borrowingHistory.push_back(*it);
                int daysOverdue = calculateOverdue(it->borrowedDate, maxBorrowingPeriod);
                if (daysOverdue > 0) {
                    calculateFine(daysOverdue);
                }
                borrowedBooks.erase(it);
                break;
            }
        }
    }

    void calculateFine(int daysOverdue) {
        overdueFine += daysOverdue * 10;
    }

    void clearFine() {
        overdueFine = 0;
        cout << "Payment done successfully. Your fine has been cleared." << endl;
    }

    bool hasOverdueBook(int maxBorrowingPeriod) const {
        time_t now = time(0);
        for (const auto& book : borrowedBooks) {
            time_t dueDate = book.borrowedDate + (maxBorrowingPeriod * 24 * 60 * 60);
            if (now > dueDate) {
                return true;
            }
        }
        return false;
    }

    int calculateOverdue(const time_t& borrowedDate, int maxBorrowingPeriod) const {
        time_t now = time(0);
        time_t dueDate = borrowedDate + (maxBorrowingPeriod * 24 * 60 * 60);
        double secondsOverdue = difftime(now, dueDate);
        int daysOverdue = secondsOverdue / (60 * 60 * 24);
        return max(0, daysOverdue);
    }

    void printBorrowedBooks() const {
        if (borrowedBooks.empty()) {
            cout << "You haven't borrowed any books yet." << endl;
        } else {
            for (const auto& book : borrowedBooks) {
                book.printDetails();
            }
        }
    }

    void printBorrowingHistory() const {
        if (borrowingHistory.empty()) {
            cout << "No borrowing history available." << endl;
        } else {
            for (const auto& book : borrowingHistory) {
                book.printDetails();
            }
        }
    }

    void printFine() const {
        cout << "Total Fine: " << overdueFine << " rupees" << endl;
    }

    void printOverdueBooksAndDays(int maxBorrowingPeriod) const {
        cout << "Overdue Books and Days:" << endl;
        time_t now = time(0);
        bool hasOverdueBooks = false;
        for (const auto& book : borrowedBooks) {
            time_t dueDate = book.borrowedDate + (maxBorrowingPeriod * 24 * 60 * 60);
            int daysOverdue = calculateOverdue(book.borrowedDate, maxBorrowingPeriod);
            if (daysOverdue > 0) {
                cout << "Book: " << book.title << ", Overdue Days: " << daysOverdue << endl;
                hasOverdueBooks = true;
            }
        }
        if (!hasOverdueBooks) {
            cout << "No overdue books." << endl;
        }
    }
};

class User {
protected:
    string name;
    string id;
    string password;
    Account account;

public:
    User() {}
    User(string n, string i, string p) : name(n), id(i), password(p) {}
    virtual ~User() {}
    virtual void borrowBook(Book& book) = 0;
    virtual void returnBook(Book& book) = 0;
    Account& getAccount() { return account; }

    void viewBorrowingHistory() const {
        account.printBorrowingHistory();
    }

    void viewBorrowedBooks() const {
        account.printBorrowedBooks();
    }

    void viewFine() const {
        account.printFine();
    }

    void payFine() {
        if (account.overdueFine == 0) {
            cout << "No dues." << endl;
        } else {
            account.clearFine();
        }
    }

    void viewOverdueBooksAndDays(int maxBorrowingPeriod) const {
        account.printOverdueBooksAndDays(maxBorrowingPeriod);
    }

    string getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    string getPassword() const {
        return password;
    }
};

class Student : public User {
public:
    Student() {}
    Student(string n, string i, string p) : User(n, i, p) {}

    void borrowBook(Book& book) override {
        if (account.borrowedBooks.size() < 3 && account.overdueFine == 0 && book.status == "Available" && !account.hasOverdueBook(15)) {
            book.status = "Borrowed";
            account.addBorrowedBook(book);
            cout << "Book borrowed successfully." << endl;
        } else {
            cout << "Cannot borrow book. Check borrowing limit, fines, overdue books, or book status." << endl;
        }
    }

    void returnBook(Book& book) override {
        auto it = find_if(account.borrowedBooks.begin(), account.borrowedBooks.end(), [&](const Book& b) { return b.ISBN == book.ISBN; });
        if (it != account.borrowedBooks.end()) {
            book.status = "Available";
            if (!book.reservedBy.empty()) {
                book.status = "Reserved";
            }
            account.returnBorrowedBook(book.ISBN, 15);
            cout << "Book returned successfully." << endl;
        } else {
            cout << "You cannot return a book you haven't borrowed." << endl;
        }
    }

    void reserveBook(Book& book) {
        if (book.status == "Borrowed" && book.reservedBy.empty() && !isBookBorrowed(book.ISBN)) {
            book.reservedBy = id;
            cout << "Book reserved successfully." << endl;
        } else {
            cout << "Cannot reserve book. Check book status or reservation status." << endl;
        }
    }

    bool isBookBorrowed(const string& ISBN) const {
        return any_of(account.borrowedBooks.begin(), account.borrowedBooks.end(),
            [&](const Book& b) { return b.ISBN == ISBN; });
    }
};

class Faculty : public User {
public:
    Faculty() {}
    Faculty(string n, string i, string p) : User(n, i, p) {}

    void borrowBook(Book& book) override {
        if (account.borrowedBooks.size() < 5 && book.status == "Available" && (!account.hasOverdueBook(30) || !hasOverdueMoreThan30Days())) {
            book.status = "Borrowed";
            account.addBorrowedBook(book);
            cout << "Book borrowed successfully." << endl;
        } else {
            cout << "Cannot borrow book. Check borrowing limit, overdue books, or book status." << endl;
        }
    }

    void returnBook(Book& book) override {
        auto it = find_if(account.borrowedBooks.begin(), account.borrowedBooks.end(), [&](const Book& b) { return b.ISBN == book.ISBN; });
        if (it != account.borrowedBooks.end()) {
            book.status = "Available";
            if (!book.reservedBy.empty()) {
                book.status = "Reserved";
            }
            account.returnBorrowedBook(book.ISBN, 30);
            cout << "Book returned successfully." << endl;
        } else {
            cout << "You cannot return a book you haven't borrowed." << endl;
        }
    }

    bool hasOverdueMoreThan30Days() const {
        time_t now = time(0);
        for (const auto& book : account.borrowedBooks) {
            time_t dueDate = book.borrowedDate + (60 * 24 * 60 * 60);
            if (now > dueDate) {
                return true;
            }
        }
        return false;
    }

    void reserveBook(Book& book) {
        if (book.status == "Borrowed" && book.reservedBy.empty() && !isBookBorrowed(book.ISBN)) {
            book.reservedBy = id;
            cout << "Book reserved successfully." << endl;
        } else {
            cout << "Cannot reserve book. Check book status or reservation status." << endl;
        }
    }

    bool isBookBorrowed(const string& ISBN) const {
        return any_of(account.borrowedBooks.begin(), account.borrowedBooks.end(),
            [&](const Book& b) { return b.ISBN == ISBN; });
    }
};

class Librarian : public User {
public:
    Librarian() {}
    Librarian(string n, string i, string p) : User(n, i, p) {}

    void borrowBook(Book& book) override {
        cout << "Librarians cannot borrow books." << endl;
    }

    void returnBook(Book& book) override {
        cout << "Librarians cannot return books." << endl;
    }

    void addBook(vector<Book>& books, const Book& book) {
        books.push_back(book);
        cout << "Book added successfully." << endl;
    }

    void removeBook(vector<Book>& books, const string& ISBN) {
        for (auto it = books.begin(); it != books.end(); ++it) {
            if (it->ISBN == ISBN) {
                books.erase(it);
                cout << "Book removed successfully." << endl;
                break;
            }
        }
    }

    void updateBook(vector<Book>& books, const string& ISBN, const Book& updatedBook) {
        for (auto& book : books) {
            if (book.ISBN == ISBN) {
                book = updatedBook;
                cout << "Book updated successfully." << endl;
                return;
            }
        }
        cout << "Book not found." << endl;
    }

    void addUser(map<string, shared_ptr<User>>& users, shared_ptr<User> user) {
        users[user->getId()] = user;
        cout << "User added successfully." << endl;
    }

    void removeUser(map<string, shared_ptr<User>>& users, const string& id) {
        users.erase(id);
        cout << "User removed successfully." << endl;
    }
};

class Library {
private:
    vector<Book> books;
    map<string, shared_ptr<User>> users;
    shared_ptr<User> currentUser = nullptr;

public:
    Library() { loadData(); }

    ~Library() { saveData(); }

    void displayBooks() const {
        if (books.empty()) {
            cout << "No book available." << endl;
        } else {
            for (const auto& book : books) {
                book.printDetails();
            }
        }
    }

    void displayUsers() const {
        for (const auto& pair : users) {
            cout << "User ID: " << pair.first << ", User Name: " << pair.second->getName() << endl;
        }
    }

    void saveData() {
        saveBooks();
        saveUsers();
        saveBorrowedBooks();
        saveBorrowingHistory();
        saveFines();
    }

    void saveBooks() {
        ofstream tempFile("books_temp.csv");
        if (!tempFile.is_open()) {
            cerr << "Error opening temporary file for books" << endl;
            return;
        }
        tempFile << "Title,Author,Publisher,Year,ISBN,Status,ReservedBy,BorrowedDate\n";
        for (const auto& book : books) {
            tempFile << "\"" << book.title << "\",\"" << book.author << "\",\"" << book.publisher << "\","
                     << book.year << "," << book.ISBN << "," << book.status << "," << book.reservedBy << ",";
            if (book.borrowedDate != 0) {
                tempFile << book.borrowedDate;
            }
            tempFile << endl;
        }
        tempFile.close();
        remove("books.csv");
        rename("books_temp.csv", "books.csv");
    }

    void saveUsers() {
        ofstream tempFile("users_temp.csv");
        if (!tempFile.is_open()) {
            cerr << "Error opening temporary file for users" << endl;
            return;
        }
        tempFile << "Name,ID,Password,Role\n";
        for (const auto& pair : users) {
            string role;
            if (dynamic_cast<Student*>(pair.second.get())) {
                role = "Student";
            } else if (dynamic_cast<Faculty*>(pair.second.get())) {
                role = "Faculty";
            } else if (dynamic_cast<Librarian*>(pair.second.get())) {
                role = "Librarian";
            }
            tempFile << pair.second->getName() << "," << pair.second->getId() << "," << pair.second->getPassword() << "," << role << endl;
        }
        tempFile.close();
        remove("users.csv");
        rename("users_temp.csv", "users.csv");
    }

    void saveBorrowedBooks() {
        ofstream tempFile("borrowed_books_temp.csv");
        if (!tempFile.is_open()) {
            cerr << "Error opening temporary file for borrowed books" << endl;
            return;
        }
        tempFile << "UserID,ISBN,BorrowedDate\n";
        for (const auto& pair : users) {
            for (const auto& book : pair.second->getAccount().borrowedBooks) {
                tempFile << pair.second->getId() << "," << book.ISBN << "," << book.borrowedDate << endl;
            }
        }
        tempFile.close();
        remove("borrowed_books.csv");
        rename("borrowed_books_temp.csv", "borrowed_books.csv");
    }

    void saveBorrowingHistory() {
        ofstream tempFile("borrowing_history_temp.csv");
        if (!tempFile.is_open()) {
            cerr << "Error opening temporary file for borrowing history" << endl;
            return;
        }
        tempFile << "UserID,ISBN,BorrowedDate\n";
        for (const auto& pair : users) {
            for (const auto& book : pair.second->getAccount().borrowingHistory) {
                tempFile << pair.second->getId() << "," << book.ISBN << "," << book.borrowedDate << endl;
            }
        }
        tempFile.close();
        remove("borrowing_history.csv");
        rename("borrowing_history_temp.csv", "borrowing_history.csv");
    }

    void saveFines() {
        ofstream tempFile("fines_temp.csv");
        if (!tempFile.is_open()) {
            cerr << "Error opening temporary file for fines" << endl;
            return;
        }
        tempFile << "UserID,FineAmount\n";
        for (const auto& pair : users) {
            tempFile << pair.second->getId() << "," << pair.second->getAccount().overdueFine << endl;
        }
        tempFile.close();
        remove("fines.csv");
        rename("fines_temp.csv", "fines.csv");
    }

    void loadData() {
        ifstream bookFile("books.csv");
        if (!bookFile.is_open()) {
            cerr << "Error opening books.csv" << endl;
            return;
        }
        string line;
        getline(bookFile, line); 
        while (getline(bookFile, line)) {
            string title, author, publisher, year, ISBN, status, reservedBy;
            time_t borrowedDate = 0;
            istringstream iss(line);
            getline(iss, title, ',');
            getline(iss, author, ',');
            getline(iss, publisher, ',');
            getline(iss, year, ',');
            getline(iss, ISBN, ',');
            getline(iss, status, ',');
            getline(iss, reservedBy, ',');
            iss >> borrowedDate;
            title = trim(title.substr(1, title.size() - 2));
            author = trim(author.substr(1, author.size() - 2));
            publisher = trim(publisher.substr(1, publisher.size() - 2));
            try {
                int yearInt = stoi(trim(year));
                books.push_back(Book(title, author, publisher, yearInt, trim(ISBN)));
                books.back().status = trim(status);
                books.back().reservedBy = trim(reservedBy);
                books.back().borrowedDate = borrowedDate;
            } catch (const std::invalid_argument& e) {
                cerr << "Invalid argument: " << e.what() << " for year: " << year << endl;
            } catch (const std::out_of_range& e) {
                cerr << "Out of range error: " << e.what() << " for year: " << year << endl;
            }
        }
        bookFile.close();
        ifstream userFile("users.csv");
        if (!userFile.is_open()) {
            cerr << "Error opening users.csv" << endl;
            return;
        }
        getline(userFile, line); 
        while (getline(userFile, line)) {
            string name, id, password, role;
            istringstream iss(line);
            getline(iss, name, ',');
            getline(iss, id, ',');
            getline(iss, password, ',');
            getline(iss, role, ',');
    
           
            name = trim(name);
            id = trim(id);
            password = trim(password);
            role = trim(role);
    
           
            if (role == "Student") {
                users[id] = make_shared<Student>(name, id, password);
            } else if (role == "Faculty") {
                users[id] = make_shared<Faculty>(name, id, password);
            } else if (role == "Librarian") {
                users[id] = make_shared<Librarian>(name, id, password);
            } else {
                cerr << "Unknown role: " << role << " for user: " << name << endl;
            }
        }
        userFile.close();
    
        ifstream borrowedBooksFile("borrowed_books.csv");
        if (!borrowedBooksFile.is_open()) {
            cerr << "Error opening borrowed_books.csv" << endl;
            return;
        }
        while (getline(borrowedBooksFile, line)) {
            string userId, ISBN;
            time_t borrowedDate;
            istringstream iss(line);
            getline(iss, userId, ',');
            getline(iss, ISBN, ',');
            iss >> borrowedDate;
            if (users.find(userId) != users.end()) {
                for (auto& book : books) {
                    if (book.ISBN == ISBN) {
                        book.status = "Borrowed";
                        book.borrowedDate = borrowedDate;
                        users[userId]->getAccount().borrowedBooks.push_back(book);
                        break;
                    }
                }
            }
        }
        borrowedBooksFile.close();
    
        ifstream borrowingHistoryFile("borrowing_history.csv");
        if (!borrowingHistoryFile.is_open()) {
            cerr << "Error opening borrowing_history.csv" << endl;
            return;
        }
        while (getline(borrowingHistoryFile, line)) {
            string userId, ISBN;
            time_t borrowedDate;
            istringstream iss(line);
            getline(iss, userId, ',');
            getline(iss, ISBN, ',');
            iss >> borrowedDate;
            if (users.find(userId) != users.end()) {
                for (auto& book : books) {
                    if (book.ISBN == ISBN) {
                        book.borrowedDate = borrowedDate;
                        users[userId]->getAccount().borrowingHistory.push_back(book);
                        break;
                    }
                }
            }
        }
        borrowingHistoryFile.close();
    
        ifstream finesFile("fines.csv");
        if (!finesFile.is_open()) {
            cerr << "Error opening fines.csv" << endl;
            return;
        }
        while (getline(finesFile, line)) {
            string userId;
            int fine;
            istringstream iss(line);
            getline(iss, userId, ',');
            iss >> fine;
            if (users.find(userId) != users.end()) {
                users[userId]->getAccount().overdueFine = fine;
            }
        }
        finesFile.close();
    }
    void login(const string& userId, const string& password) {
        if (users.find(userId) != users.end() && users[userId]->getPassword() == password) {
            currentUser = users[userId];
            cout << "Logged in as " << currentUser->getName() << endl;
        } else {
            cout << "Invalid User ID or Password." << endl;
        }
    }

    void logout() {
        currentUser = nullptr;
        cout << "Logged out." << endl;
    }

    User* getCurrentUser() const {
        return currentUser.get();
    }

    void borrowBook(const string& ISBN) {
        if (currentUser) {
            for (auto& book : books) {
                if (book.ISBN == ISBN && book.status == "Available") {
                    currentUser->borrowBook(book);
                    saveData(); 
                    return;
                }
            }
            cout << "Book not available." << endl;
        } else {
            cout << "Please login first." << endl;
        }
    }

    void returnBook(const string& ISBN) {
        if (currentUser) {
            for (auto& book : books) {
                if (book.ISBN == ISBN) {
                    currentUser->returnBook(book);
                    saveData(); 
                    return;
                }
            }
            cout << "Book not found." << endl;
        } else {
            cout << "Please login first." << endl;
        }
    }

    void reserveBook(const string& ISBN) {
        if (currentUser) {
            for (auto& book : books) {
                if (book.ISBN == ISBN) {
                    if (dynamic_cast<Student*>(currentUser.get())) {
                        dynamic_cast<Student*>(currentUser.get())->reserveBook(book);
                    } else if (dynamic_cast<Faculty*>(currentUser.get())) {
                        dynamic_cast<Faculty*>(currentUser.get())->reserveBook(book);
                    }
                    saveData(); 
                    return;
                }
            }
            cout << "Book not found." << endl;
        } else {
            cout << "Please login first." << endl;
        }
    }                                             
    void adminAddBook(const Book& newBook) {
        if (currentUser && dynamic_cast<Librarian*>(currentUser.get())) {
            dynamic_cast<Librarian*>(currentUser.get())->addBook(books, newBook);
            saveData(); 
        } else {
            cout << "You do not have permission to add books." << endl;
        }
    }

    void adminRemoveBook(const string& ISBN) {
        if (currentUser && dynamic_cast<Librarian*>(currentUser.get())) {
            dynamic_cast<Librarian*>(currentUser.get())->removeBook(books, ISBN);
            saveData(); 
        } else {
            cout << "You do not have permission to remove books." << endl;
        }
    }

    void adminUpdateBook(const string& ISBN, const Book& updatedBook) {
        if (currentUser && dynamic_cast<Librarian*>(currentUser.get())) {
            dynamic_cast<Librarian*>(currentUser.get())->updateBook(books, ISBN, updatedBook);
            saveData(); 
        } else {
            cout << "You do not have permission to update books." << endl;
        }
    }

    void adminAddUser(shared_ptr<User> newUser) {
        if (currentUser && dynamic_cast<Librarian*>(currentUser.get())) {
            dynamic_cast<Librarian*>(currentUser.get())->addUser(users, newUser);
            saveData(); 
        } else {
            cout << "You do not have permission to add users." << endl;
        }
    }

 
    void adminRemoveUser(const string& userId) {
        if (currentUser && dynamic_cast<Librarian*>(currentUser.get())) {
            dynamic_cast<Librarian*>(currentUser.get())->removeUser(users, userId);
            saveData(); 
        } else {
            cout << "You do not have permission to remove users." << endl;
        }
    }
    void studentDashboard() {
        int choice;
        string ISBN;
        while (true) {
            cout << "\n--- STUDENT DASHBOARD (" << currentUser->getName() << ") ---\n";
            cout << "1. View Available Books\n";
            cout << "2. Borrow a Book\n";
            cout << "3. Return a Book\n";
            cout << "4. View Borrowed Books\n";
            cout << "5. Check Fine Amount\n";
            cout << "6. Pay Fine\n";
            cout << "7. Reserve a Book\n";
            cout << "8. View Borrowing History\n";
            cout << "9. View Overdue Books and Days\n";
            cout << "10. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
                case 1:
                    displayBooks();
                    break;
                case 2:
                    cout << "Enter Book ISBN: ";
                    cin >> ISBN;
                    borrowBook(ISBN);
                    break;
                case 3:
                    cout << "Enter Book ISBN: ";
                    cin >> ISBN;
                    returnBook(ISBN);
                    break;
                case 4:
                    currentUser->viewBorrowedBooks();
                    break;
                case 5:
                    currentUser->viewFine();
                    break;
                case 6:
                    currentUser->payFine();
                    saveData(); 
                    break;
                case 7:
                    cout << "Enter Book ISBN: ";
                    cin >> ISBN;
                    reserveBook(ISBN);
                    break;
                case 8:
                    currentUser->viewBorrowingHistory();
                    break;
                case 9:
                    currentUser->viewOverdueBooksAndDays(15);
                    break;
                case 10:
                    logout();
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
    void facultyDashboard() {
        int choice;
        string ISBN;
        while (true) {
            cout << "\n--- FACULTY DASHBOARD (" << currentUser->getName() << ") ---\n";
            cout << "1. View Available Books\n";
            cout << "2. Borrow a Book\n";
            cout << "3. Return a Book\n";
            cout << "4. View Borrowed Books\n";
            cout << "5. Reserve a Book\n";
            cout << "6. View Borrowing History\n";
            cout << "7. View Overdue Books and Days\n";
            cout << "8. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
                case 1:
                    displayBooks();
                    break;
                case 2:
                    cout << "Enter Book ISBN: ";
                    cin >> ISBN;
                    borrowBook(ISBN);
                    break;
                case 3:
                    cout << "Enter Book ISBN: ";
                    cin >> ISBN;
                    returnBook(ISBN);
                    break;
                case 4:
                    currentUser->viewBorrowedBooks();
                    break;
                case 5:
                    cout << "Enter Book ISBN: ";
                    cin >> ISBN;
                    reserveBook(ISBN);
                    break;
                case 6:
                    currentUser->viewBorrowingHistory();
                    break;
                case 7:
                    currentUser->viewOverdueBooksAndDays(30); 
                    break;
                case 8:
                    logout();
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
    void librarianDashboard() {
        int choice;
        string ISBN, name, id, role, password;
        while (true) {
            cout << "\n--- LIBRARIAN DASHBOARD (" << currentUser->getName() << ") ---\n";
            cout << "1. View Books\n";
            cout << "2. Add Book\n";
            cout << "3. Remove Book\n";
            cout << "4. Update Book\n";
            cout << "5. Add User\n";
            cout << "6. Remove User\n";
            cout << "7. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
                case 1:
                    displayBooks();
                    break;
                case 2: {
                    string title, author, publisher;
                    int year;
                    cout << "Enter Book Title: ";
                    cin.ignore();
                    getline(cin, title);
                    cout << "Enter Book Author: ";
                    getline(cin, author);
                    cout << "Enter Book Publisher: ";
                    getline(cin, publisher);
                    cout << "Enter Book Year: ";
                    cin >> year;
                    cout << "Enter Book ISBN: ";
                    cin >> ISBN;
                    adminAddBook(Book(title, author, publisher, year, ISBN));
                    break;
                }
                case 3:
                    cout << "Enter Book ISBN: ";
                    cin >> ISBN;
                    adminRemoveBook(ISBN);
                    break;
                case 4: {
                    string title, author, publisher;
                    int year;
                    cout << "Enter Book ISBN to Update: ";
                    cin >> ISBN;
                    cout << "Enter New Book Title: ";
                    cin.ignore();
                    getline(cin, title);
                    cout << "Enter New Book Author: ";
                    getline(cin, author);
                    cout << "Enter New Book Publisher: ";
                    getline(cin, publisher);
                    cout << "Enter New Book Year: ";
                    cin >> year;
                    adminUpdateBook(ISBN, Book(title, author, publisher, year, ISBN));
                    break;
                }
                case 5:
                    cout << "Enter User Name: ";
                    cin.ignore();
                    getline(cin, name);
                    cout << "Enter User ID: ";
                    cin >> id;
                    cout << "Enter User Password: ";
                    cin >> password;
                    cout << "Enter User Role (Student/Faculty): ";
                    cin >> role;
                    if (role == "Student") {
                        adminAddUser(make_shared<Student>(name, id, password));
                    } else if (role == "Faculty") {
                        adminAddUser(make_shared<Faculty>(name, id, password));
                    } else {
                        cout << "Invalid Role. Please enter either 'Student' or 'Faculty'." << endl;
                        break;
                    }
                    break;
                case 6:
                    cout << "Enter User ID: ";
                    cin >> id;
                    adminRemoveUser(id);
                    break;
                case 7:
                    logout();
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
};
int main() {
Library library;
int choice;
string userId, password;
while (true) {
    cout << "\nLibrary Management System\n";
    cout << "1. Login\n";
    cout << "2. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;
    switch (choice) {
        case 1:
            cout << "Enter User ID: ";
            cin >> userId;
            cout << "Enter Password: ";
            cin >> password;
            library.login(userId, password);
            if (library.getCurrentUser()) {
                if (dynamic_cast<Student*>(library.getCurrentUser())) {
                    library.studentDashboard();
                } else if (dynamic_cast<Faculty*>(library.getCurrentUser())) {
                    library.facultyDashboard();
                } else if (dynamic_cast<Librarian*>(library.getCurrentUser())) {
                    library.librarianDashboard();
                }
            }
            break;
        case 2:
            library.saveData();
            cout << "Data saved. Exiting..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }
}
return 0;
}

