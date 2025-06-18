#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <conio.h>
#include <cctype>
#include <cstdlib>
#include <stdexcept>
#include <windows.h>

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
    string username;
    int userID;
    int bookID;
    string bookName;
    string lendTime; 
    string returnTime; 	
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
        char* dt = ctime(&now);
        return string(dt);
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
        ofstream outfile("users.txt", ios::app);
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
                // Update nextBookID to the highest book ID + 1
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
        cout << setw(5) << "ID" << setw(20) << "Name" << setw(20) << "Author" 
             << setw(10) << "Pages" << setw(15) << "Year Released" << setw(15) 
             << "Available Copies" << endl;
        cout << "----------------------------------------------------------------------------------------" << endl;
        for (const auto& book : library) {
            cout << setw(5) << book.id << setw(20) << book.name << setw(20) << book.author 
                 << setw(10) << book.pages << setw(15) << book.yearReleased << setw(15) 
                 << book.availableCopies << endl;
        }
    }

    void viewBookDetails() {
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
        } catch (const invalid_argument& e) {
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
        } catch (const out_of_range& e) {
        cout << "Input number is out of range" << endl;
        }
    }
        
    void addBook() {
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
        string bookID;
        cout << "Enter book ID or Name delete: ";
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
        } catch (const invalid_argument& e) {
            for (auto it = library.begin(); it != library.end(); ++it) {
                if (it->name == bookID) {
                    library.erase(it);
                    saveBooks();
                    cout << "Book deleted successfully!" << endl;
                    return;
                }
            }
            
        } catch (const out_of_range& e) {
        cout << "Input number is out of range." << endl;
        }
        cout << "Book not found!" << endl;
    }

    void editBookAmount() {
        string bookID;
        int newAmount;
        cout << "Enter book ID or Name edit: "; 
        cin >> bookID;
        try {
            int i = stoi(bookID);
            for (auto& book : library) {
                if (book.id == i) {
                    cout << "Enter new amount: "; 
                    cin >> newAmount;
                    book.availableCopies = newAmount;
                    saveBooks();
                    cout << "Book amount updated successfully!" << endl;
                    return;
                }
            }
        } catch (const invalid_argument& e) {
            for (auto& book : library) {
                if (book.name == bookID) {
                    cout << "Enter new amount: "; 
                    cin >> newAmount;
                    book.availableCopies = newAmount;
                    saveBooks();
                    cout << "Book amount updated successfully!" << endl;
                    return;
                }
            }
        } catch (const out_of_range& e) {
        cout << "Input number is out of range" << endl;
        }
        
        cout << "Book not found!" << endl;
    }

    void lendBook() {
        string bookID;
        cout << "Enter book ID to lend: "; 
        cin >> bookID;
        try {
            int i = stoi(bookID);
            for (auto& book : library) {
                if (book.id == i && book.availableCopies > 0) {
                    book.availableCopies--;
                    saveBooks();
                    cout << "Book lent successfully!" << endl;
                    return;
            }
        }
        } catch (const invalid_argument& e) {
            for (auto& book : library) {
                if (book.name == bookID && book.availableCopies > 0) {
                    book.availableCopies--;
                    saveBooks();
                    cout << "Book lent successfully!" << endl;
                    return;
            }
            }
        } catch (const out_of_range& e) {
        cout << "Input number is out of range" << endl;
        }
        
        cout << "Book not found or no copies available!" << endl;
    }

    void returnBook() {
        string bookID;
        cout << "Enter book ID or Name return: "; 
        cin >> bookID;
        try {
            int i = stoi(bookID);
            for (auto& book : library) {
                if (book.id == i) {
                    book.availableCopies++;
                    saveBooks();
                    cout << "Book returned successfully!" << endl;
                    return;
            }
        }
        
        } catch (const invalid_argument& e) {
            for (auto& book : library) {
                if (book.name == bookID) {
                    book.availableCopies++;
                    saveBooks();
                    cout << "Book returned successfully!" << endl;
                    return;
                }
            }
        } catch (const out_of_range& e) {
        cout << "Input number is out of range." << endl;
        }
        cout << "Book not found!" << endl;
    }

    void manageLibrary() {
        int choice = 1;

        while (choice != 0) {
            cout << "1. View Book List" << endl;
            cout << "2. View Book Details" << endl;
            cout << "3. Add Book" << endl;
            cout << "4. Delete Book" << endl;
            cout << "5. Edit Book Amount" << endl;
            cout << "6. Lend Book" << endl;
            cout << "7. Return Book" << endl;
            cout << "0. Logout" << endl;
            cout << "Choose the option: "; 
            cin >> choice;
            system("cls");

            switch (choice) {
                case 1:
                    viewBookList();
                    break;
                case 2:
                    viewBookDetails();
                    break;
                case 3:
                    addBook();
                    break;
                case 4:
                    deleteBook();
                    break;
                case 5:
                    editBookAmount();
                    break;
                case 6:
                    lendBook();
                    break;
                case 7:
                    returnBook();
                    break;
                case 0:
                    cout << "Logging out..." << endl;
                    break;
                default:
                    cout << "Invalid choice!" << endl;
                    break;
            }
        }
    }
};
int main() {
    LibraryManager manager;
    manager.loadUsers();
    manager.loadBooks();

    int choice = 1;
    while (choice != 0) {
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "0. Exit" << endl;
        cout << "Choose the option: "; 
        cin >> choice;
        system("cls");

        switch (choice) {
            case 1:
                manager.registerUser();
                break;
            case 2:
                if (manager.login()) {
                    manager.manageLibrary();
                }
                break;
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
                break;
        }
    }
    return 0;
}