#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include <vector>
#include <memory>
#include "book.hpp"
#include "user.hpp"

class Library {
private:
    std::vector<std::unique_ptr<Book>> books;
    std::vector<std::unique_ptr<User>> users;
    
    void loadData();
    void saveData() const;
    
public:
    Library();
    ~Library();
    
    // Book management
    bool addBook(std::unique_ptr<Book> book);
    bool removeBook(const std::string& bookId);
    Book* findBook(const std::string& bookId);
    std::vector<Book*> searchBooks(const std::string& query);
    
    // User management
    bool addUser(std::unique_ptr<User> user);
    bool removeUser(const std::string& userId);
    User* findUser(const std::string& userId);
    const std::vector<std::unique_ptr<User>>& getAllUsers() const; // Added this method
    
    // Borrowing operations
    bool borrowBook(const std::string& userId, const std::string& bookId);
    bool returnBook(const std::string& userId, const std::string& bookId);
    
    // Fine management
    double calculateFine(const std::string& userId, const std::string& bookId);
    void clearFine(const std::string& userId);
    
    // Data persistence
    void initialize();
};

#endif
