#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <conio.h>
#include <ctime>
#include <stdexcept>
#include <windows.h>
#include <sstream>	

using namespace std;

struct User {
    string username;
    string password;
};

struct Book {
    int id;
    string name;
    string author;
    int pages;
    int yearReleased;
    int availableCopies;
};

struct LentBook {
    int bookID;
    string bookName;
    string lendTime;
    string returnTime;
    string studentName;
    int studentID;
};

vector<Book> library;
vector<User> users;
vector<LentBook> lentBooks;

class LibraryManager {
private:
    string loggedInUser;
    int nextBookID;

    string getCurrentTime() {
        time_t now = time(0);
        char buffer[100];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d-%H:%M", localtime(&now));  
        return string(buffer);
    }

public:
    LibraryManager() : loggedInUser(""), nextBookID(1) {}

    void loadUsers() {
        ifstream infile("users.txt");
        if (infile.is_open()) {
            User user;
            while (infile >> user.username >> user.password) {
                users.push_back(user);
            }
            infile.close();
        }
    }

    void saveUsers() {
        ofstream outfile("users.txt"); 
        if (outfile.is_open()) {
            for (const auto& user : users) {
                outfile << user.username << " " << user.password << endl;
            }
            outfile.close();
        }
    }

    void loadBooks() {
        ifstream infile("books.txt");
        if (infile.is_open()) {
            Book book;
            while (infile >> book.id >> book.name >> book.author >> book.pages >> book.yearReleased >> book.availableCopies) {
                library.push_back(book);
                if (book.id >= nextBookID) {
                    nextBookID = book.id + 1;
                }
            }
            infile.close();
        }
    }

    void saveBooks() {
        ofstream outfile("books.txt");
        if (outfile.is_open()) {
            for (const auto& book : library) {
                outfile << book.id << " " << book.name << " " << book.author << " "
                        << book.pages << " " << book.yearReleased << " "
                        << book.availableCopies << endl;
            }
            outfile.close();
        }
    }

    void loadLentBooks() {
        ifstream infile("lent_books.txt");
        if (infile.is_open()) {
            LentBook lent;
            while (infile >> lent.bookID >> lent.bookName >> lent.studentName>> lent.studentID >> lent.lendTime >>lent.returnTime){
               if (lent.returnTime.empty() || lent.returnTime == "Not-Returned") {
                lent.returnTime = "Not-Returned";
				}
	
            lentBooks.push_back(lent);  
            }
            infile.close();
        } else {
            cerr << "Unable to open file 'lent_books.txt'" << endl;
        }
    }

    void saveLentBooks() {
        ofstream outfile("lent_books.txt");
        if (outfile.is_open()) {
            for (const auto& lent : lentBooks) {
                outfile << lent.bookID << " "
                        << lent.bookName << " "
                        << lent.studentName << " " 
                        << lent.studentID <<" "
                        << lent.lendTime << " "
                        << lent.returnTime << endl;
            }
            outfile.close();
        } else {
            cerr << "Unable to open file 'lent_books.txt'" << endl;
        }
    }

    string inputWithMask() {
        string input;
        char ch;
        while ((ch = _getch()) != '\r') {
            if (ch == '\b' && !input.empty()) {
                cout << "\b \b";
                input.pop_back();
            } else {
                input.push_back(ch);
                cout << '*';
            }
        }
        cout << endl;
        return input;
    }

    bool authenticate(const string& username, const string& password) {
        for (const auto& user : users) {
            if (user.username == username && user.password == password) {
                return true;
            }
        }
        return false;
    }

    bool login() {
        system("cls");  // Clear screen for a clean login prompt
        string username, password;

        cout << "Enter username: ";
        cin.ignore();
        getline(cin, username);

        cout << "Enter password: ";
        password = inputWithMask();

        if (authenticate(username, password)) {
            cout << "Login successful." << endl;
            loggedInUser = username;
            return true;
        } else {
            cout << "Invalid username or password." << endl;
            return false;
        }
    }

    void registerUser() {
        system("cls");  // Clear screen before registration prompt
        User newUser;
        cout << "Enter new username: ";
        cin >> newUser.username;
        cout << "Enter new password: ";
        newUser.password = inputWithMask();
        users.push_back(newUser);
        saveUsers();
        cout << "User registered successfully!" << endl;
    }

    void viewBookList() {
        system("cls");  // Clear screen before displaying book list
        cout << setw(5) << "ID" << setw(20) << "Name" << setw(20) << "Author"
             << setw(10) << "Pages" << setw(20) << "Year Released" << setw(20)
             << "Available Copies" << endl;
        cout << "----------------------------------------------------------------------------------------------------------" << endl;
        for (const auto& book : library) {
            cout << setw(5) << book.id << setw(20) << book.name << setw(20) << book.author
                 << setw(10) << book.pages << setw(20) << book.yearReleased << setw(20)
                 << book.availableCopies << endl;
        }
    }

    void viewBookDetails() {
        system("cls");
        string bookName;
        cout << "Enter book ID or Name: ";
        cin.ignore();
        getline(cin, bookName);
        try {
            int i = stoi(bookName);
            for (auto& book : library) {
                if (book.id == i) {
                    cout << "Name: " << book.name << endl;
                    cout << "Author: " << book.author << endl;
                    cout << "Pages: " << book.pages << endl;
                    cout << "Year Released: " << book.yearReleased << endl;
                    cout << "Available Copies: " << book.availableCopies << endl;
                    return;
                }
            }
        } catch (const invalid_argument&) {
            for (auto& book : library) {
                if (book.name == bookName) {
                    cout << "Name: " << book.name << endl;
                    cout << "Author: " << book.author << endl;
                    cout << "Pages: " << book.pages << endl;
                    cout << "Year Released: " << book.yearReleased << endl;
                    cout << "Available Copies: " << book.availableCopies << endl;
                    return;
                }
            }
        }
        cout << "Book not found!" << endl;
    }

    void addBook() {
        system("cls");  
        Book newBook;
        newBook.id = nextBookID++;
        cout << "Enter book name: ";
        cin.ignore();
        getline(cin, newBook.name);
        cout << "Enter author name: ";
        getline(cin, newBook.author);
        cout << "Enter number of pages: ";
        cin >> newBook.pages;
        cout << "Enter year released: ";
        cin >> newBook.yearReleased;
        cout << "Enter available copies: ";
        cin >> newBook.availableCopies;
        library.push_back(newBook);
        saveBooks();
        cout << "Book added successfully!" << endl;
    }

    void deleteBook() {
        system("cls");  // Clear screen before deleting a book
        string bookID;
        cout << "Enter book ID or Name to delete: ";
        cin >> bookID;
        try {
            int i = stoi(bookID);
            for (auto it = library.begin(); it != library.end(); ++it) {
                if (it->id == i) {
                    library.erase(it);
                    saveBooks();
                    cout << "Book deleted successfully!" << endl;
                    return;
                }
            }
        } catch (const invalid_argument&) {
            for (auto it = library.begin(); it != library.end(); ++it) {
                if (it->name == bookID) {
                    library.erase(it);
                    saveBooks();
                    cout << "Book deleted successfully!" << endl;
                    return;
                }
            }
        }
        cout << "Book not found!" << endl;
    }

    void lendBook() {         
    system("cls");  // Clear screen before lending a book         
    string bookID;         
    cout << "Enter book ID or Name to lend: ";         
    cin.ignore();         
    getline(cin, bookID);          

    try {             
        int i = stoi(bookID); // Try to convert bookID to an integer             
        for (auto& book : library) {                 
            if (book.id == i) {                     
                if (book.availableCopies > 0) {                         
                    book.availableCopies--;                         
                    LentBook newLentBook;                         

                    // Gather student information
                    cout << "Enter student name: ";                         
                    getline(cin, newLentBook.studentName);                          

                    cout << "Enter student ID: ";                         
                    cin >> newLentBook.studentID;                          
                    cin.ignore();  
                    
                    // Set lending details
                    newLentBook.bookID = book.id;                         
                    newLentBook.bookName = book.name;                         
                    newLentBook.lendTime = getCurrentTime();                         
                    newLentBook.returnTime = "Not-Returned";                          

                    // Add lent book entry and save updates
                    lentBooks.push_back(newLentBook);                         
                    saveBooks();                         
                    saveLentBooks();                         
                    cout << "Book lent successfully!" << endl;                         
                    return;                     
                } else {                         
                    cout << "No available copies to lend." << endl;                         
                    return;                     
                }                 
            }             
        }         
    } catch (const invalid_argument& e) {             
        for (auto& book : library) {                 
            if (book.name == bookID) {                     
                if (book.availableCopies > 0) {                         
                    book.availableCopies--;                         
                    LentBook newLentBook;                         

                    // Gather student information
                    cout << "Enter student name: ";                         
                    getline(cin, newLentBook.studentName);                          

                    cout << "Enter student ID: ";                         
                    cin >> newLentBook.studentID;                          
                    cin.ignore();  // Clear newline for future input

                    // Set lending details
                    newLentBook.bookID = book.id;                         
                    newLentBook.bookName = book.name;                         
                    newLentBook.lendTime = getCurrentTime();                         
                    newLentBook.returnTime = "Not-Returned";                          

                    // Add lent book entry and save updates
                    lentBooks.push_back(newLentBook);                         
                    saveBooks();                         
                    saveLentBooks();                         
                    cout << "Book lent successfully!" << endl;                         
                    return;                     
                } else {                         
                    cout << "No available copies to lend." << endl;                         
                    return;                     
                }                 
            }             
        }         
    }          

    cout << "Book not found!" << endl;     
}
    void returnBook() {
        system("cls"); 
        int studentID;
        string bookID;
        cout << "Enter student ID: ";
        cin >> studentID;
        cout << "Enter book ID or Name to return: ";
        cin.ignore();
        getline(cin, bookID);

        for (auto& lent : lentBooks) {
            if (lent.studentID == studentID && (lent.bookName == bookID || to_string(lent.bookID) == bookID)) {
                lent.returnTime = getCurrentTime();  

                for (auto& book : library) {
                    if (book.id == lent.bookID) {
                        book.availableCopies++;
                        break;
                    }
                }

                saveLentBooks();
                saveBooks();
                cout << "Book returned successfully!" << endl;
                return;
            }
        }

        cout << "Lent book record not found!" << endl;
    }

    void viewLentBooks() {
        system("cls"); 
        cout << setw(5) << "ID" << setw(20) << "Name" << setw(20) << "Student Name"
             << setw(20) << "Student ID" << setw(20) << "Lend Time" << setw(20) << "Return Time" << endl;
        cout << "----------------------------------------------------------------------------------------------------------------" << endl;
        for (const auto& lent : lentBooks) {
            cout << setw(5) << lent.bookID << setw(20) << lent.bookName << setw(20)
                 << lent.studentName << setw(20) << lent.studentID <<setw(20)
				 << lent.lendTime << setw(23) <<lent.returnTime << endl;
        }
    }
};

int main() {
    LibraryManager manager;

    manager.loadUsers();
    manager.loadBooks();
    manager.loadLentBooks();

    while (true) {
        int choice;
        system("cls");  // Clear screen before displaying main menu
        cout << "Library Management System\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            manager.registerUser();
        } else if (choice == 2) {
            if (manager.login()) {
                int userChoice;
                do {
                    system("cls");  // Clear screen before displaying user menu
                    cout << "\nLibrary Menu\n";
                    cout << "1. View Book List\n";
                    cout << "2. View Book Details\n";
                    cout << "3. Add Book\n";
                    cout << "4. Delete Book\n";
                    cout << "5. Lend Book\n";
                    cout << "6. Return Book\n";
                    cout << "7. View Lent Books\n";
                    cout << "8. Logout\n";
                    cout << "Enter choice: ";
                    cin >> userChoice;

                    switch (userChoice) {
                    case 1:
                        manager.viewBookList();
                        break;
                    case 2:
                        manager.viewBookDetails();
                        break;
                    case 3:
                        manager.addBook();
                        break;
                    case 4:
                        manager.deleteBook();
                        break;
                    case 5:
                        manager.lendBook();
                        break;
                    case 6:
                        manager.returnBook();
                        break;
                    case 7:
                        manager.viewLentBooks();
                        break;
                    case 8:
                        cout << "Logging out...\n";
                        break;
                    default:
                        cout << "Invalid choice!\n";
                    }
                    system("pause");  
                } while (userChoice != 8);
            }
        } else if (choice == 3) {
            cout << "Exiting the system.\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
