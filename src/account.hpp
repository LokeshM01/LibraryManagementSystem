#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <string>
#include <vector>
#include <ctime>

struct BorrowRecord {
    std::string bookId;
    std::time_t borrowDate;
    std::time_t returnDate;
};

class Account {
private:
    std::vector<std::string> currentlyBorrowedBooks;
    std::vector<BorrowRecord> borrowHistory;
    double outstandingFine;

public:
    Account();
    
    bool addBorrowedBook(const std::string& bookId);
    bool returnBook(const std::string& bookId);
    bool hasFine() const;
    void addFine(double amount);
    void clearFine();
    double getFine() const;
    const std::vector<std::string>& getCurrentlyBorrowedBooks() const;
    const std::vector<BorrowRecord>& getBorrowHistory() const;
    
    std::string serialize() const;
    static Account deserialize(const std::string& data);
};

#endif
