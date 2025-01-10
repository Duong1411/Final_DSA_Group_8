#include <iostream>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

// Book class to store book data
class Book {
public:
    string title;
    string author;
    string ISBN;
    string category; // New attribute to store book category

    // Constructor
    Book(string t, string a, string i, string c) {
        title = t;
        author = a;
        ISBN = i;
        category = c;
    }

    // Default constructor
    Book() {
        title = "";
        author = "";
        ISBN = "";
        category = "";
    }
};

class BorrowRecord {
public:
    string ISBN;
    string borrowerName;
    string borrowDate;
    int borrowDays;

    // Constructor
    BorrowRecord(string i, string name, string date, int days) {
        ISBN = i;
        borrowerName = name;
        borrowDate = date;
        borrowDays = days;
    }

    // Default constructor
    BorrowRecord() {
        ISBN = "";
        borrowerName = "";
        borrowDate = "";
        borrowDays = 0;
    }
};

class Library {
private:
    unordered_map<string, Book> books; // Hash table for storing books, key: ISBN
    vector<BorrowRecord> borrowList;  // List for managing borrowed books
    string fileName = "library_data.txt"; // File to store book data
    string borrowFile = "borrow_data.txt";


    void saveToFile() {
        ofstream outFile(fileName);
        if (outFile.is_open()) {
            for (const auto& pair : books) {
                outFile << pair.second.title << "," << pair.second.author << "," << pair.second.ISBN << "," << pair.second.category << endl;
            }
            outFile.close();
        } else {
            cout << "Error: Unable to open file for writing." << endl;
        }
    }

    void saveBorrowListToFile() {
        ofstream outFile(borrowFile);
        if (outFile.is_open()) {
            for (const auto& record : borrowList) {
                outFile << record.ISBN << "," << record.borrowerName << "," << record.borrowDate << "," << record.borrowDays << endl;
            }
            outFile.close();
        } else {
            cout << "Error: Unable to open file for writing." << endl;
        }
    }


    void loadFromFile() {
        ifstream inFile(fileName);
        if (inFile.is_open()) {
            books.clear();
            string line;
            while (getline(inFile, line)) {
                size_t pos1 = line.find(',');
                size_t pos2 = line.find(',', pos1 + 1);
                size_t pos3 = line.find(',', pos2 + 1);
                if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
                    string title = line.substr(0, pos1);
                    string author = line.substr(pos1 + 1, pos2 - pos1 - 1);
                    string ISBN = line.substr(pos2 + 1, pos3 - pos2 - 1);
                    string category = line.substr(pos3 + 1);
                    books[ISBN] = Book(title, author, ISBN, category);
                }
            }
            inFile.close();
        }
    }

    void loadBorrowListFromFile() {
        ifstream inFile(borrowFile);
        if (inFile.is_open()) {
            borrowList.clear();
            string line;
            while (getline(inFile, line)) {
                size_t pos1 = line.find(',');
                size_t pos2 = line.find(',', pos1 + 1);
                size_t pos3 = line.find(',', pos2 + 1);
                if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
                    string ISBN = line.substr(0, pos1);
                    string borrowerName = line.substr(pos1 + 1, pos2 - pos1 - 1);
                    string borrowDate = line.substr(pos2 + 1, pos3 - pos2 - 1);
                    int borrowDays = stoi(line.substr(pos3 + 1));
                    borrowList.push_back(BorrowRecord(ISBN, borrowerName, borrowDate, borrowDays));
                }
            }
            inFile.close();
        }
    }

public:
    Library() {
        loadFromFile();
        loadBorrowListFromFile();
    }

    // Add a book to the library
    void addBook(const Book& book) {
        if (books.find(book.ISBN) == books.end()) {
            books[book.ISBN] = book;
            cout << "Book added to the library." << endl;
            saveToFile();
        } else {
            cout << "Error: Book with this ISBN already exists." << endl;
        }
    }

    // Delete a book from the library
    void deleteBook(const string& ISBN) {
        if (books.erase(ISBN)) {
            cout << "Book deleted from the library." << endl;
            saveToFile();
        } else {
            cout << "Error: Book not found." << endl;
        }
    }

    // Update book information
    void updateBook(const string& ISBN, const string& newTitle, const string& newAuthor, const string& newCategory) {
        if (books.find(ISBN) != books.end()) {
            books[ISBN].title = newTitle;
            books[ISBN].author = newAuthor;
            books[ISBN].category = newCategory;
            cout << "Book information updated." << endl;
            saveToFile();
        } else {
            cout << "Error: Book not found." << endl;
        }
    }

    // Search for books by category
    void displayBooksByCategory(const string& category) {
        bool found = false;
        cout << left << setw(35) << "Title" << setw(25) << "Author" << setw(20) << "ISBN" << setw(20) << "Category" << endl;
        cout << string(100, '-') << endl;
        for (const auto& pair : books) {
            if (pair.second.category == category) {
                cout << left << setw(35) << pair.second.title
                     << setw(25) << pair.second.author
                     << setw(20) << pair.second.ISBN
                     << setw(20) << pair.second.category << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "No books found in category: " << category << endl;
        }
    }

    // Search for books by title
    void searchByTitle(const string& title) {
        bool found = false;
        cout << left << setw(35) << "Title" << setw(25) << "Author" << setw(20) << "ISBN" << setw(20) << "Category" << endl;
        cout << string(100, '-') << endl;
        for (const auto& pair : books) {
            if (pair.second.title == title) {
                cout << left << setw(35) << pair.second.title
                     << setw(25) << pair.second.author
                     << setw(20) << pair.second.ISBN
                     << setw(20) << pair.second.category << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "No books found with title: " << title << endl;
        }
    }

    // Search for books by ISBN
    void searchByISBN(const string& ISBN) {
        auto it = books.find(ISBN);
        if (it != books.end()) {
            cout << left << setw(35) << "Title" << setw(25) << "Author" << setw(20) << "ISBN" << setw(20) << "Category" << endl;
            cout << string(100, '-') << endl;
            cout << left << setw(35) << it->second.title
                 << setw(25) << it->second.author
                 << setw(20) << it->second.ISBN
                 << setw(20) << it->second.category << endl;
        } else {
            cout << "No books found with ISBN: " << ISBN << endl;
        }
    }

    // Display all books
    void displayAllBooks() {
        cout << left << setw(35) << "Title" << setw(25) << "Author" << setw(20) << "ISBN" << setw(20) << "Category" << endl;
        cout << string(100, '-') << endl;
        for (const auto& pair : books) {
            cout << left << setw(35) << pair.second.title
                 << setw(25) << pair.second.author
                 << setw(20) << pair.second.ISBN
                 << setw(20) << pair.second.category << endl;
        }
    }

    void addBorrowRecord() {
        string ISBN, borrowerName, borrowDate;
        int borrowDays;

        cout << "Enter ISBN of the book: ";
        cin.ignore();
        getline(cin, ISBN);
        if (books.find(ISBN) == books.end()) {
            cout << "Error: Book not found." << endl;
            return;
        }

        cout << "Enter borrower name: ";
        getline(cin, borrowerName);
        cout << "Enter borrow date (YYYY-MM-DD): ";
        getline(cin, borrowDate);
        cout << "Enter number of days to borrow: ";
        cin >> borrowDays;

        BorrowRecord record(ISBN, borrowerName, borrowDate, borrowDays);
        borrowList.push_back(record);
        saveBorrowListToFile();
        cout << "Borrow record added successfully." << endl;
    }

    void viewBorrowList() {
        cout << left << setw(20) << "ISBN" << setw(25) << "Borrower" << setw(15) << "Borrow Date" << setw(10) << "Days" << endl;
        cout << string(70, '-') << endl;
        for (const auto& record : borrowList) {
            cout << left << setw(20) << record.ISBN
                 << setw(25) << record.borrowerName
                 << setw(15) << record.borrowDate
                 << setw(10) << record.borrowDays << endl;
        }
    }

    void manageReturns() {
        string borrowerName, ISBN;
        int overdueDays;
        double conditionPenalty = 0;
        string condition;

        cout << "Enter borrower's name: ";
        cin.ignore();
        getline(cin, borrowerName);
        cout << "Enter ISBN of the book: ";
        getline(cin, ISBN);

        auto it = find_if(borrowList.begin(), borrowList.end(), [&](const BorrowRecord& record) {
            return record.borrowerName == borrowerName && record.ISBN == ISBN;
        });

        if (it != borrowList.end()) {
            cout << "Enter number of overdue days: ";
            cin >> overdueDays;
            cout << "Enter condition of the book (100%, 75%, 50%, 25%): ";
            cin >> condition;

            if (condition == "100%") {
                conditionPenalty = 0;
            } else if (condition == "75%") {
                conditionPenalty = 3;
            } else if (condition == "50%") {
                conditionPenalty = 5;
            } else if (condition == "25%") {
                conditionPenalty = 8;
            }

            double totalPenalty = overdueDays * 2 + conditionPenalty;
            cout << "Total penalty: $" << totalPenalty << endl;

            borrowList.erase(it);
            saveBorrowListToFile();
            cout << "Book returned successfully." << endl;
        } else {
            cout << "Error: Borrow record not found." << endl;
        }
    }

    void borrowMenu() {
        int choice;
        do {
            system("cls");
            cout << "\nBorrow Management System" << endl;
            cout << "1. Add Borrow Record" << endl;
            cout << "2. View Borrow List" << endl;
            cout << "3. Manage Returns" << endl;
            cout << "4. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    addBorrowRecord();
                    break;
                case 2:
                    viewBorrowList();
                    break;
                case 3:
                    manageReturns();
                    break;
                case 4:
                    cout << "Exiting borrow management." << endl;
                    break;
                default:
                    cout << "Invalid choice. Try again." << endl;
            }
            system("pause");
        } while (choice != 4);
    }


    void libraryMenu() {
        int choice;
        do {
            system("cls");
            cout << "\nLibrary Management System" << endl;
            cout << "1. Add Book" << endl;
            cout << "2. Delete Book" << endl;
            cout << "3. Update Book" << endl;
            cout << "4. Search Book by Category" << endl;
            cout << "5. Search Book by Title" << endl;
            cout << "6. Search Book by ISBN" << endl;
            cout << "7. Display All Books" << endl;
            cout << "8. Borrow Management" << endl;
            cout << "9. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    string title, author, ISBN, category;
                    cout << "Enter title: ";
                    cin.ignore();
                    getline(cin, title);
                    cout << "Enter author: ";
                    getline(cin, author);
                    cout << "Enter ISBN: ";
                    getline(cin, ISBN);
                    cout << "Enter category: ";
                    getline(cin, category);
                    addBook(Book(title, author, ISBN, category));
                    break;
                }
                case 2: {
                    string ISBN;
                    cout << "Enter ISBN to delete: ";
                    cin.ignore();
                    getline(cin, ISBN);
                    deleteBook(ISBN);
                    break;
                }
                case 3: {
                    string ISBN, newTitle, newAuthor, newCategory;
                    cout << "Enter ISBN to update: ";
                    cin.ignore();
                    getline(cin, ISBN);
                    cout << "Enter new title: ";
                    getline(cin, newTitle);
                    cout << "Enter new author: ";
                    getline(cin, newAuthor);
                    cout << "Enter new category: ";
                    getline(cin, newCategory);
                    updateBook(ISBN, newTitle, newAuthor, newCategory);
                    break;
                }
                case 4: {
                    string category;
                    cout << "Enter category to search: ";
                    cin.ignore();
                    getline(cin, category);
                    displayBooksByCategory(category);
                    break;
                }
                case 5: {
                    string title;
                    cout << "Enter title to search: ";
                    cin.ignore();
                    getline(cin, title);
                    searchByTitle(title);
                    break;
                }
                case 6: {
                    string ISBN;
                    cout << "Enter ISBN to search: ";
                    cin.ignore();
                    getline(cin, ISBN);
                    searchByISBN(ISBN);
                    break;
                }
                case 7:
                    displayAllBooks();
                    break;
                case 8:
                    borrowMenu();
                    break;
                case 9:
                    cout << "Exiting library management." << endl;
                    break;
                default:
                    cout << "Invalid choice. Try again." << endl;
            }
            system("pause");
        } while (choice != 9);
    }
};

class Admin {
private:
    string adminFile = "admin_data.txt";

    bool verifyCredentials(const string& username, const string& password) {
        ifstream inFile(adminFile);
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                size_t pos = line.find(',');
                if (pos != string::npos) {
                    string storedUsername = line.substr(0, pos);
                    string storedPassword = line.substr(pos + 1);
                    if (storedUsername == username && storedPassword == password) {
                        inFile.close();
                        return true;
                    }
                }
            }
            inFile.close();
        }
        return false;
    }

public:
    void login() {
        string username, password;
        bool loggedIn = false;
        while (!loggedIn) {
            system("cls");
            cout << "===== Admin Login =====" << endl;
            cout << "Enter admin username: ";
            cin >> username;
            cout << "Enter admin password: ";
            cin >> password;

            if (verifyCredentials(username, password)) {
                cout << "Login successful!" << endl;
                loggedIn = true;
            } else {
                cout << "Invalid credentials. Please try again." << endl;
                system("pause");
            }
        }
    }

    void createAccount() {
        string username, password;
        system("cls");
        cout << "===== Create Admin Account =====" << endl;
        cout << "Enter new admin username: ";
        cin >> username;
        cout << "Enter new admin password: ";
        cin >> password;

        ofstream outFile(adminFile, ios::app);
        if (outFile.is_open()) {
            outFile << username << "," << password << endl;
            outFile.close();
            cout << "Admin account created successfully. Returning to login...\n" << endl;
        } else {
            cout << "Error: Unable to open file for writing." << endl;
        }
    }
};

int main() {
    Admin admin;
    Library library;
    int option;

    while (true) {
        system("cls");
        cout << "1. Login\n2. Create Admin Account\nChoose an option: ";
        cin >> option;

        if (option == 1) {
            admin.login();
            library.libraryMenu();
            break;
        } else if (option == 2) {
            admin.createAccount();
        } else {
            cout << "Invalid option. Please try again." << endl;
            system("pause");
        }
    }

    return 0;
}
