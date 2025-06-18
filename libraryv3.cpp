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
const int COLOR_DEFAULT = 7;
const int COLOR_RED = 12;
const int COLOR_GREEN = 10;
const int COLOR_BLUE = 9;

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
    void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
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
        setColor(COLOR_GREEN);
        cout << "Enter username: ";
        cin.ignore();
        getline(cin, username);

        cout << "Enter password: ";
        password = inputWithMask();

        if (authenticate(username, password)) {
          setColor(COLOR_GREEN); 
            cout << "Login successful." << endl;
            loggedInUser = username;
            setColor(COLOR_DEFAULT);
            return true;
        } else {
             setColor(COLOR_RED);
            cout << "Invalid username or password." << endl;
            setColor(COLOR_DEFAULT);
            return false;
        }
    }

    void registerUser() {
        system("cls");  // Clear screen before registration prompt
        User newUser;
        setColor(COLOR_GREEN);
        cout << "Enter new username: ";
        cin >> newUser.username;
        cout << "Enter new password: ";
        newUser.password = inputWithMask();
        users.push_back(newUser);
        saveUsers();
        setColor(COLOR_GREEN);
        cout << "User registered successfully!" << endl;
        setColor(COLOR_DEFAULT);
    }

    void viewBookList() {
        system("cls");  
        setColor(COLOR_BLUE);
        cout << setw(5) << "ID" << setw(20) << "Name" << setw(20) << "Author"
             << setw(10) << "Pages" << setw(20) << "Year Released" << setw(20)
             << "Available Copies" << endl;
        cout << "----------------------------------------------------------------------------------------------------------" << endl;
        setColor(COLOR_DEFAULT);
        for (const auto& book : library) {
            cout << setw(5) << book.id << setw(20) << book.name << setw(20) << book.author
                 << setw(10) << book.pages << setw(20) << book.yearReleased << setw(20)
                 << book.availableCopies << endl;
        }
                 
    }

    void viewBookDetails() {
        system("cls");
        string bookName;
        setColor(COLOR_GREEN);
        cout << "Enter book ID or Name: ";
        cin.ignore();
        getline(cin, bookName);
        try {
            int i = stoi(bookName);
            for (auto& book : library) {
                if (book.id == i) {
                    setColor(COLOR_DEFAULT);
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
                    setColor(COLOR_BLUE);
                    cout << "Name: " << book.name << endl;
                    cout << "Author: " << book.author << endl;
                    cout << "Pages: " << book.pages << endl;
                    cout << "Year Released: " << book.yearReleased << endl;
                    cout << "Available Copies: " << book.availableCopies << endl;
                    return;
                }
                    setColor(COLOR_DEFAULT);
            }
        }
        setColor(COLOR_RED); 
        cout << "Book not found!" << endl;
        setColor(COLOR_DEFAULT);
    }

    void addBook() {
        system("cls");  
        Book newBook;
        setColor(COLOR_BLUE);
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
        setColor(COLOR_GREEN);
        cout << "Book added successfully!" << endl;
        setColor(COLOR_DEFAULT);
    }

    void deleteBook() {
        system("cls");  // Clear screen before deleting a book
        string bookID;
        setColor(COLOR_BLUE);
        cout << "Enter book ID or Name to delete: ";
        cin >> bookID;
        try {
            int i = stoi(bookID);
            for (auto it = library.begin(); it != library.end(); ++it) {
                if (it->id == i) {
                    library.erase(it);
                    saveBooks();
                    setColor(COLOR_GREEN);
                    cout << "Book deleted successfully!" << endl;
                    setColor(COLOR_DEFAULT);
                    return;
                }
            }
        } catch (const invalid_argument&) {
            for (auto it = library.begin(); it != library.end(); ++it) {
                if (it->name == bookID) {
                    library.erase(it);
                    saveBooks();
                    setColor(COLOR_GREEN);
                    cout << "Book deleted successfully!" << endl;
                    setColor(COLOR_DEFAULT);
                    return;
                }
            }
        }
        setColor(COLOR_RED);
        cout << "Book not found!" << endl;
        setColor(COLOR_DEFAULT);
    }

    void editBookAmount() {
    	system("cls");
    int bookID, newAmount;
    string bookName;
    string input;
    setColor(COLOR_BLUE);
    cout << "Enter book ID or book name to edit: ";
    cin.ignore();
    getline(cin, input);

    try {
        bookID = stoi(input);
        for (auto& book : library) {
            if (book.id == bookID) {
                setColor(COLOR_BLUE);
                cout << "Enter new amount: ";
                cin >> newAmount;
                book.availableCopies = newAmount;
                saveBooks();
                setColor(COLOR_GREEN);
                cout << "Book amount updated successfully!" << endl;
                setColor(COLOR_DEFAULT);
                return;
            }
        }
    } catch (const invalid_argument&) {
        for (auto& book : library) {
            if (book.name == input) {
                setColor(COLOR_BLUE);
                cout << "Enter new amount: ";
                cin >> newAmount;
                book.availableCopies = newAmount;
                saveBooks();
                setColor(COLOR_GREEN);
                cout << "Book amount updated successfully!" << endl;
                setColor(COLOR_DEFAULT);
                return;
            }
        }
        setColor(COLOR_RED);
        cout << "Book name not found." << endl;
        setColor(COLOR_DEFAULT);
    }
}
//===================================================================================================
    void lendBook() {         
    system("cls");  // Clear screen before lending a book         
    string bookID;       
    setColor(COLOR_BLUE);  
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
                    setColor(COLOR_BLUE);
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
                    setColor(COLOR_GREEN);               
                    cout << "Book lent successfully!" << endl;   
                    setColor(COLOR_DEFAULT);                      
                    return;                     
                } else {   
                    setColor(COLOR_RED);                      
                    cout << "No available copies to lend." << endl;     
                    setColor(COLOR_DEFAULT);                    
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
                    setColor(COLOR_BLUE);
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
                    setColor(COLOR_GREEN);                        
                    cout << "Book lent successfully!" << endl;
                    setColor(COLOR_DEFAULT);                        
                    return;                     
                } else {  
                    setColor(COLOR_RED);                       
                    cout << "No available copies to lend." << endl;  
                    setColor(COLOR_DEFAULT);                       
                    return;                     
                }                 
            }             
        }         
    }          
     setColor(COLOR_RED);    
    cout << "Book not found!" << endl;
     setColor(COLOR_DEFAULT);         
}
    void returnBook() {
        system("cls"); 
        int studentID;
        string bookID;
         setColor(COLOR_BLUE);    
        cout << "Enter student ID: ";
        cin >> studentID;
        cout << "Enter book ID or Name to return: ";
        cin.ignore();
        getline(cin, bookID);
         setColor(COLOR_DEFAULT);    
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
                 setColor(COLOR_GREEN);    
                cout << "Book returned successfully!" << endl;
                 setColor(COLOR_DEFAULT);    
                return;
            }
        }
         setColor(COLOR_RED);    
        cout << "Lent book record not found!" << endl;
         setColor(COLOR_DEFAULT);    
    }

    void viewLentBooks() {
        system("cls"); 
             setColor(COLOR_DEFAULT);  
        cout << setw(5) << "ID" << setw(20) << "Name" << setw(20) << "Student Name"
             << setw(20) << "Student ID" << setw(20) << "Lend Time" << setw(20) << "Return Time" << endl;
        cout << "----------------------------------------------------------------------------------------------------------------" << endl;
        for (const auto& lent : lentBooks) {
            if (lent.returnTime.empty() || lent.returnTime == "Not-Returned") {
                 setColor(COLOR_RED);  
                }else{setColor(COLOR_BLUE); }
            
            cout << setw(5) << lent.bookID << setw(20) << lent.bookName << setw(20)
                 << lent.studentName << setw(20) << lent.studentID <<setw(20)
				 << lent.lendTime << setw(23) <<lent.returnTime << endl;
        }
            setColor(COLOR_DEFAULT);    
    }
};

int main() {
    LibraryManager manager;

    manager.loadUsers();
    manager.loadBooks();
    manager.loadLentBooks();
    

    while (true) {
        int choice;
        system("cls"); 
        cout << "Library Management System\n";
        manager.setColor(COLOR_BLUE);
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
         manager.setColor(COLOR_GREEN);  
        cout << "Enter choice: ";
        manager.setColor(COLOR_DEFAULT);
        cin >> choice;

        if (choice == 1) {
            manager.registerUser();
        } else if (choice == 2) {
            if (manager.login()) {
                int userChoice;
                do {
                    system("cls");  // Clear screen before displaying user menu
                    manager.setColor(COLOR_BLUE); 
                    cout << "\nLibrary Menu\n";
                    manager.setColor(COLOR_DEFAULT);    
                    cout << "1. View Book List\n";
                    cout << "2. Search Book & View Details\n";
                    cout << "3. Add Book\n";
                    cout << "4. Delete Book\n";
                    cout << "5. Edit Book Amount\n";
                    cout << "6. Lend Book\n";
                    cout << "7. Return Book\n";
                    cout << "8. View Lent Books\n";
                    cout << "9. Logout\n";
                    manager.setColor(COLOR_GREEN); 
                    cout << "Enter choice: ";
                    manager.setColor(COLOR_DEFAULT);    
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
                        manager.editBookAmount();
                        break;
                    case 6:
                        manager.lendBook();
                        break;
                    case 7:
                        manager.returnBook();
                        break;
                    case 8:
                        manager.viewLentBooks();
                        break;
                    case 9:
                        cout << "Logging out...\n";
                        break;
                    default:
                        cout << "Invalid choice!\n";
                    }
                    system("pause");  
                } while (userChoice != 9);
            }
        } else if (choice == 3) {

            cout << "Exiting the system......\n";
            break;
        } else {
             manager.setColor(COLOR_RED);    
            cout << "Invalid choice. Try again.\n";
            manager.setColor(COLOR_DEFAULT); 
        }
    }

    return 0;
}
