#include "user.hpp"
#include "utils.hpp"
#include <sstream>

User::User(const std::string& name, const std::string& email, 
           const std::string& password, UserRole role)
    : id(Utils::generateUniqueId())
    , name(name)
    , email(email)
    , password(password)
    , role(role) {}

std::string User::getId() const { return id; }
std::string User::getName() const { return name; }
std::string User::getEmail() const { return email; }
UserRole User::getRole() const { return role; }
Account& User::getAccount() { return account; }

std::string User::serialize() const {
    std::stringstream ss;
    ss << static_cast<int>(role) << "|" << id << "|" << name << "|" 
       << email << "|" << password << "|" << account.serialize();
    return ss.str();
}

User* User::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string roleStr, id, name, email, password, accountData;
    
    std::getline(ss, roleStr, '|');
    std::getline(ss, id, '|');
    std::getline(ss, name, '|');
    std::getline(ss, email, '|');
    std::getline(ss, password, '|');
    std::getline(ss, accountData);
    
    UserRole role = static_cast<UserRole>(std::stoi(roleStr));
    User* user = nullptr;
    
    switch(role) {
        case UserRole::STUDENT:
            user = new Student(name, email, password);
            break;
        case UserRole::FACULTY:
            user = new Faculty(name, email, password);
            break;
        case UserRole::LIBRARIAN:
            user = new Librarian(name, email, password);
            break;
    }
    
    if (user) {
        user->id = id;
        user->account = Account::deserialize(accountData);
    }
    
    return user;
}

Student::Student(const std::string& name, const std::string& email, 
                const std::string& password)
    : User(name, email, password, UserRole::STUDENT) {}

double Student::calculateFine(int daysOverdue) const {
    return daysOverdue > 0 ? daysOverdue * 10.0 : 0.0;
}

Faculty::Faculty(const std::string& name, const std::string& email, 
                const std::string& password)
    : User(name, email, password, UserRole::FACULTY) {}

double Faculty::calculateFine(int daysOverdue) const {
    return 0.0;
}

Librarian::Librarian(const std::string& name, const std::string& email, 
                     const std::string& password)
    : User(name, email, password, UserRole::LIBRARIAN) {}

double Librarian::calculateFine(int daysOverdue) const {
    return 0.0;
}
