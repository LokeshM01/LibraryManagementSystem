#include "library.hpp"
#include "utils.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream> // Added for debug output

Library::Library() {
    std::cout << "Initializing Library System...\n";
    loadData();
}

Library::~Library() {
    std::cout << "Saving Library System state...\n";
    saveData();
}

void Library::loadData() {
    std::cout << "Loading data from files...\n";
    // Load books
    std::string bookData = Utils::readFromFile("data/books.txt");
    std::stringstream bookSS(bookData);
    std::string line;
    while (std::getline(bookSS, line)) {
        if (!line.empty()) {
            books.push_back(std::make_unique<Book>(Book::deserialize(line)));
            std::cout << "Loaded book: " << books.back()->getTitle() << "\n";
        }
    }

    // Load users
    std::string userData = Utils::readFromFile("data/users.txt");
    std::stringstream userSS(userData);
    while (std::getline(userSS, line)) {
        if (!line.empty()) {
            users.push_back(std::unique_ptr<User>(User::deserialize(line)));
            std::cout << "Loaded user: " << users.back()->getName() << "\n";
        }
    }
}

void Library::saveData() const {
    std::cout << "Saving data to files...\n";
    // Save books
    std::stringstream bookSS;
    for (const auto& book : books) {
        bookSS << book->serialize() << "\n";
    }
    Utils::saveToFile("data/books.txt", bookSS.str());

    // Save users
    std::stringstream userSS;
    for (const auto& user : users) {
        userSS << user->serialize() << "\n";
    }
    Utils::saveToFile("data/users.txt", userSS.str());
    std::cout << "Data saved successfully.\n";
}

bool Library::addBook(std::unique_ptr<Book> book) {
    books.push_back(std::move(book));
    return true;
}

bool Library::removeBook(const std::string& bookId) {
    auto it = std::find_if(books.begin(), books.end(),
        [&bookId](const auto& book) { return book->getId() == bookId; });

    if (it != books.end()) {
        books.erase(it);
        return true;
    }
    return false;
}

Book* Library::findBook(const std::string& bookId) {
    auto it = std::find_if(books.begin(), books.end(),
        [&bookId](const auto& book) { return book->getId() == bookId; });

    return it != books.end() ? it->get() : nullptr;
}

std::vector<Book*> Library::searchBooks(const std::string& query) {
    std::vector<Book*> results;
    for (const auto& book : books) {
        if (book->getTitle().find(query) != std::string::npos ||
            book->getAuthor().find(query) != std::string::npos ||
            book->getIsbn().find(query) != std::string::npos) {
            results.push_back(book.get());
        }
    }
    return results;
}

bool Library::addUser(std::unique_ptr<User> user) {
    users.push_back(std::move(user));
    return true;
}

bool Library::removeUser(const std::string& userId) {
    auto it = std::find_if(users.begin(), users.end(),
        [&userId](const auto& user) { return user->getId() == userId; });

    if (it != users.end()) {
        users.erase(it);
        return true;
    }
    return false;
}

User* Library::findUser(const std::string& userId) {
    auto it = std::find_if(users.begin(), users.end(),
        [&userId](const auto& user) { return user->getId() == userId; });

    return it != users.end() ? it->get() : nullptr;
}

bool Library::borrowBook(const std::string& userId, const std::string& bookId) {
    User* user = findUser(userId);
    Book* book = findBook(bookId);

    if (!user || !book) {
        std::cout << "Error: User or book not found.\n";
        return false;
    }
    if (book->getStatus() != BookStatus::AVAILABLE) {
        std::cout << "Error: Book is not available.\n";
        return false;
    }
    if (user->getAccount().getCurrentlyBorrowedBooks().size() >= user->getMaxBooks()) {
        std::cout << "Error: User has reached maximum books limit.\n";
        return false;
    }
    if (user->getAccount().hasFine()) {
        std::cout << "Error: User has outstanding fines.\n";
        return false;
    }

    book->setStatus(BookStatus::BORROWED);
    user->getAccount().addBorrowedBook(bookId);
    std::cout << "Book '" << book->getTitle() << "' borrowed by " << user->getName() << "\n";
    return true;
}

bool Library::returnBook(const std::string& userId, const std::string& bookId) {
    User* user = findUser(userId);
    Book* book = findBook(bookId);

    if (!user || !book) {
        std::cout << "Error: User or book not found.\n";
        return false;
    }

    book->setStatus(BookStatus::AVAILABLE);
    user->getAccount().returnBook(bookId);

    double fine = calculateFine(userId, bookId);
    if (fine > 0) {
        user->getAccount().addFine(fine);
        std::cout << "Fine of ₹" << fine << " added for overdue book.\n";
    }

    std::cout << "Book '" << book->getTitle() << "' returned by " << user->getName() << "\n";
    return true;
}

double Library::calculateFine(const std::string& userId, const std::string& bookId) {
    User* user = findUser(userId);
    if (!user) return 0.0;

    for (const auto& record : user->getAccount().getBorrowHistory()) {
        if (record.bookId == bookId && record.returnDate > 0) {
            int daysOverdue = Utils::calculateDaysDifference(
                record.borrowDate + user->getMaxDays() * 24 * 60 * 60,
                record.returnDate);
            return user->calculateFine(daysOverdue);
        }
    }
    return 0.0;
}

void Library::clearFine(const std::string& userId) {
    User* user = findUser(userId);
    if (user) {
        user->getAccount().clearFine();
    }
}

void Library::initialize() {
    // Add initial books
    addBook(std::make_unique<Book>("The Great Gatsby", "F. Scott Fitzgerald", "Scribner", 1925, "978-0743273565"));
    addBook(std::make_unique<Book>("To Kill a Mockingbird", "Harper Lee", "Grand Central", 1960, "978-0446310789"));
    addBook(std::make_unique<Book>("1984", "George Orwell", "Penguin", 1949, "978-0451524935"));
    addBook(std::make_unique<Book>("Pride and Prejudice", "Jane Austen", "Penguin", 1813, "978-0141439518"));
    addBook(std::make_unique<Book>("The Catcher in the Rye", "J.D. Salinger", "Little, Brown", 1951, "978-0316769488"));

    // Add initial users
    addUser(std::make_unique<Student>("John Doe", "john@example.com", "pass123"));
    addUser(std::make_unique<Student>("Jane Smith", "jane@example.com", "pass456"));
    addUser(std::make_unique<Faculty>("Dr. Wilson", "wilson@example.com", "pass789"));
    addUser(std::make_unique<Librarian>("Admin", "admin@library.com", "admin123"));
}

const std::vector<std::unique_ptr<User>>& Library::getAllUsers() const {
    return users;
}