#include "book.hpp"
#include "utils.hpp"
#include <sstream>

Book::Book(const std::string& title, const std::string& author, 
           const std::string& publisher, int year, const std::string& isbn)
    : id(Utils::generateUniqueId())
    , title(title)
    , author(author)
    , publisher(publisher)
    , year(year)
    , isbn(isbn)
    , status(BookStatus::AVAILABLE) {}

std::string Book::getId() const { return id; }
std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
std::string Book::getPublisher() const { return publisher; }
int Book::getYear() const { return year; }
std::string Book::getIsbn() const { return isbn; }
BookStatus Book::getStatus() const { return status; }

void Book::setStatus(BookStatus newStatus) {
    status = newStatus;
}

std::string Book::serialize() const {
    std::stringstream ss;
    ss << id << "|" << title << "|" << author << "|" 
       << publisher << "|" << year << "|" << isbn << "|" 
       << static_cast<int>(status);
    return ss.str();
}

Book Book::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string id, title, author, publisher, isbn, statusStr, yearStr;
    
    std::getline(ss, id, '|');
    std::getline(ss, title, '|');
    std::getline(ss, author, '|');
    std::getline(ss, publisher, '|');
    std::getline(ss, yearStr, '|');
    std::getline(ss, isbn, '|');
    std::getline(ss, statusStr, '|');
    
    Book book(title, author, publisher, std::stoi(yearStr), isbn);
    book.id = id;
    book.status = static_cast<BookStatus>(std::stoi(statusStr));
    
    return book;
}
