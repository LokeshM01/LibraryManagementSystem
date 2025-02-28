#include "account.hpp"
#include "utils.hpp"
#include <sstream>
#include <algorithm>

Account::Account() : outstandingFine(0.0) {}

bool Account::addBorrowedBook(const std::string& bookId) {
    currentlyBorrowedBooks.push_back(bookId);
    BorrowRecord record{bookId, Utils::getCurrentTime(), 0};
    borrowHistory.push_back(record);
    return true;
}

bool Account::returnBook(const std::string& bookId) {
    auto it = std::find(currentlyBorrowedBooks.begin(), 
                       currentlyBorrowedBooks.end(), bookId);
    if (it != currentlyBorrowedBooks.end()) {
        currentlyBorrowedBooks.erase(it);

        // Update return date in history
        for (auto& record : borrowHistory) {
            if (record.bookId == bookId && record.returnDate == 0) {
                record.returnDate = Utils::getCurrentTime();
                break;
            }
        }
        return true;
    }
    return false;
}

bool Account::hasFine() const {
    return outstandingFine > 0;
}

void Account::addFine(double amount) {
    outstandingFine += amount;
}

void Account::clearFine() {
    outstandingFine = 0;
}

double Account::getFine() const {
    return outstandingFine;
}

const std::vector<std::string>& Account::getCurrentlyBorrowedBooks() const {
    return currentlyBorrowedBooks;
}

const std::vector<BorrowRecord>& Account::getBorrowHistory() const {
    return borrowHistory;
}

std::string Account::serialize() const {
    std::stringstream ss;

    ss << outstandingFine << ";";

    // Serialize currently borrowed books
    ss << currentlyBorrowedBooks.size() << ";";
    for (const auto& bookId : currentlyBorrowedBooks) {
        ss << bookId << ",";
    }
    ss << ";";

    // Serialize borrow history
    ss << borrowHistory.size() << ";";
    for (const auto& record : borrowHistory) {
        ss << record.bookId << "," 
           << record.borrowDate << "," 
           << record.returnDate << ";";
    }

    return ss.str();
}

Account Account::deserialize(const std::string& data) {
    Account account;
    std::stringstream ss(data);
    std::string segment;

    // Get fine
    std::getline(ss, segment, ';');
    account.outstandingFine = std::stod(segment);

    // Get currently borrowed books
    std::getline(ss, segment, ';');
    int currentSize = std::stoi(segment);
    std::getline(ss, segment, ';');
    if (currentSize > 0) {
        std::stringstream bookSS(segment);
        std::string bookId;
        while (std::getline(bookSS, bookId, ',')) {
            if (!bookId.empty()) {
                account.currentlyBorrowedBooks.push_back(bookId);
            }
        }
    }

    // Get borrow history
    std::getline(ss, segment, ';');
    int historySize = std::stoi(segment);
    for (int i = 0; i < historySize; ++i) {
        std::getline(ss, segment, ';');
        std::stringstream recordSS(segment);
        std::string bookId, borrowDate, returnDate;
        std::getline(recordSS, bookId, ',');
        std::getline(recordSS, borrowDate, ',');
        std::getline(recordSS, returnDate, ',');

        BorrowRecord record{
            bookId,
            static_cast<std::time_t>(std::stoll(borrowDate)),
            static_cast<std::time_t>(std::stoll(returnDate))
        };
        account.borrowHistory.push_back(record);
    }

    return account;
}