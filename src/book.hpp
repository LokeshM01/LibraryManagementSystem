#ifndef BOOK_HPP
#define BOOK_HPP

#include <string>

enum class BookStatus {
    AVAILABLE,
    BORROWED,
    RESERVED
};

class Book {
private:
    std::string id;
    std::string title;
    std::string author;
    std::string publisher;
    int year;
    std::string isbn;
    BookStatus status;

public:
    Book(const std::string& title, const std::string& author, 
         const std::string& publisher, int year, const std::string& isbn);
    
    // Getters
    std::string getId() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getPublisher() const;
    int getYear() const;
    std::string getIsbn() const;
    BookStatus getStatus() const;
    
    // Setters
    void setStatus(BookStatus newStatus);
    
    // Serialization
    std::string serialize() const;
    static Book deserialize(const std::string& data);
};

#endif
