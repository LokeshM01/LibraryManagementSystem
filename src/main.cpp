#include <iostream>
#include <iomanip>
#include <limits>
#include "library.hpp"

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void displayMainMenu() {
    std::cout << "\n=== Library Management System ===\n"
              << "1. Login\n"
              << "2. Exit\n"
              << "Choice: ";
}

void displayUserMenu(UserRole role) {
    clearScreen();
    std::cout << "\n=== User Menu ===\n";
    
    switch(role) {
        case UserRole::STUDENT:
        case UserRole::FACULTY:
            std::cout << "1. View Available Books\n"
                     << "2. Search Books\n"
                     << "3. Borrow Book\n"
                     << "4. Return Book\n"
                     << "5. View My Books\n"
                     << "6. View My Fines\n"
                     << "7. Pay Fine\n"
                     << "8. Logout\n";
            break;
            
        case UserRole::LIBRARIAN:
            std::cout << "1. Add Book\n"
                     << "2. Remove Book\n"
                     << "3. Add User\n"
                     << "4. Remove User\n"
                     << "5. View All Books\n"
                     << "6. View All Users\n"
                     << "7. View All Fines\n"
                     << "8. Logout\n";
            break;
    }
    std::cout << "Choice: ";
}

void displayBooks(const std::vector<Book*>& books) {
    std::cout << "\n=== Books ===\n";
    std::cout << std::setw(10) << "ID" 
              << std::setw(30) << "Title"
              << std::setw(20) << "Author"
              << std::setw(15) << "Status\n";
    std::cout << std::string(75, '-') << "\n";
    
    for (const auto& book : books) {
        std::string status;
        switch(book->getStatus()) {
            case BookStatus::AVAILABLE: status = "Available"; break;
            case BookStatus::BORROWED: status = "Borrowed"; break;
            case BookStatus::RESERVED: status = "Reserved"; break;
        }
        
        std::cout << std::setw(10) << book->getId()
                  << std::setw(30) << book->getTitle()
                  << std::setw(20) << book->getAuthor()
                  << std::setw(15) << status << "\n";
    }
}

User* login(Library& library) {
    std::string email, password;
    std::cout << "Email: ";
    std::cin >> email;
    std::cout << "Password: ";
    std::cin >> password;

    for (const auto& user : library.getAllUsers()) {
        if (user->getEmail() == email && user->getPassword() == password) { 
            return user.get();
        }
    }
    return nullptr;
}

void handleStudentFacultyMenu(Library& library, User* user) {
    int choice;
    std::string bookId;
    
    while (true) {
        displayUserMenu(user->getRole());
        std::cin >> choice;
        
        switch(choice) {
            case 1: 
                {
                    auto books = library.searchBooks("");
                    displayBooks(books);
                }
                break;
                
            case 2: 
                {
                    std::string query;
                    std::cout << "Enter search term: ";
                    std::cin >> query;
                    auto books = library.searchBooks(query);
                    displayBooks(books);
                }
                break;
                
            case 3: 
                {
                    std::cout << "Enter Book ID: ";
                    std::cin >> bookId;
                    if (library.borrowBook(user->getId(), bookId)) {
                        std::cout << "Book borrowed successfully!\n";
                    } else {
                        std::cout << "Failed to borrow book.\n";
                    }
                }
                break;
                
            case 4: 
                {
                    std::cout << "Enter Book ID: ";
                    std::cin >> bookId;
                    if (library.returnBook(user->getId(), bookId)) {
                        std::cout << "Book returned successfully!\n";
                    } else {
                        std::cout << "Failed to return book.\n";
                    }
                }
                break;
                
            case 5: 
                {
                    const auto& borrowed = user->getAccount().getCurrentlyBorrowedBooks();
                    std::cout << "\nCurrently Borrowed Books:\n";
                    for (const auto& bookId : borrowed) {
                        if (auto book = library.findBook(bookId)) {
                            std::cout << book->getTitle() << "\n";
                        }
                    }
                }
                break;
                
            case 6: 
                std::cout << "Outstanding fine: ₹" << user->getAccount().getFine() << "\n";
                break;
                
            case 7: 
                if (user->getAccount().hasFine()) {
                    library.clearFine(user->getId());
                    std::cout << "Fine paid successfully!\n";
                } else {
                    std::cout << "No outstanding fines.\n";
                }
                break;
                
            case 8: 
                return;
                
            default:
                std::cout << "Invalid choice!\n";
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
}

void handleLibrarianMenu(Library& library, User* user) {
    int choice;
    std::string input;
    
    while (true) {
        displayUserMenu(user->getRole());
        std::cin >> choice;
        
        switch(choice) {
            case 1: 
                {
                    std::string title, author, publisher, isbn;
                    int year;
                    
                    std::cin.ignore();
                    std::cout << "Title: ";
                    std::getline(std::cin, title);
                    std::cout << "Author: ";
                    std::getline(std::cin, author);
                    std::cout << "Publisher: ";
                    std::getline(std::cin, publisher);
                    std::cout << "Year: ";
                    std::cin >> year;
                    std::cout << "ISBN: ";
                    std::cin >> isbn;
                    
                    library.addBook(std::make_unique<Book>(title, author, publisher, year, isbn));
                    std::cout << "Book added successfully!\n";
                }
                break;
                
            case 2: 
                {
                    std::cout << "Enter Book ID: ";
                    std::cin >> input;
                    if (library.removeBook(input)) {
                        std::cout << "Book removed successfully!\n";
                    } else {
                        std::cout << "Failed to remove book.\n";
                    }
                }
                break;
                
            case 3: 
                {
                    std::string name, email, password;
                    int roleChoice;
                    
                    std::cin.ignore();
                    std::cout << "Name: ";
                    std::getline(std::cin, name);
                    std::cout << "Email: ";
                    std::cin >> email;
                    std::cout << "Password: ";
                    std::cin >> password;
                    std::cout << "Role (1: Student, 2: Faculty): ";
                    std::cin >> roleChoice;
                    
                    std::unique_ptr<User> newUser;
                    if (roleChoice == 1) {
                        newUser = std::make_unique<Student>(name, email, password);
                    } else {
                        newUser = std::make_unique<Faculty>(name, email, password);
                    }
                    
                    library.addUser(std::move(newUser));
                    std::cout << "User added successfully!\n";
                }
                break;
                
            case 4: 
                {
                    std::cout << "Enter User ID: ";
                    std::cin >> input;
                    if (library.removeUser(input)) {
                        std::cout << "User removed successfully!\n";
                    } else {
                        std::cout << "Failed to remove user.\n";
                    }
                }
                break;
                
            case 5: 
                {
                    auto books = library.searchBooks("");
                    displayBooks(books);
                }
                break;
                
            case 6: 
                {
                    std::cout << "\n=== Users ===\n";
                    for (const auto& u : library.getAllUsers()) {
                        std::cout << "ID: " << u->getId() 
                                << ", Name: " << u->getName()
                                << ", Role: " << static_cast<int>(u->getRole()) << "\n";
                    }
                }
                break;
                
            case 7: 
                {
                    std::cout << "\n=== Outstanding Fines ===\n";
                    for (const auto& u : library.getAllUsers()) {
                        if (u->getAccount().hasFine()) {
                            std::cout << "User: " << u->getName() 
                                    << ", Fine: ₹" << u->getAccount().getFine() << "\n";
                        }
                    }
                }
                break;
                
            case 8: 
                return;
                
            default:
                std::cout << "Invalid choice!\n";
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }
}

int main() {
    Library library;
    library.initialize();
    
    while (true) {
        displayMainMenu();
        int choice;
        std::cin >> choice;
        
        if (choice == 2) break;
        
        if (choice == 1) {
            User* user = login(library);
            if (user) {
                std::cout << "Welcome, " << user->getName() << "!\n";
                
                if (user->getRole() == UserRole::LIBRARIAN) {
                    handleLibrarianMenu(library, user);
                } else {
                    handleStudentFacultyMenu(library, user);
                }
            } else {
                std::cout << "Invalid credentials!\n";
            }
        }
    }
    
    std::cout << "Thank you for using the Library Management System!\n";
    return 0;
}