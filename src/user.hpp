#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
#include "account.hpp"

enum class UserRole {
    STUDENT,
    FACULTY,
    LIBRARIAN
};

class User {
protected:
    std::string id;
    std::string name;
    std::string email;
    std::string password;
    UserRole role;
    Account account;

public:
    User(const std::string& name, const std::string& email, 
         const std::string& password, UserRole role);
    
    virtual ~User() = default;
    
    // Getters
    std::string getId() const;
    std::string getName() const;
    std::string getEmail() const;
    UserRole getRole() const;
    Account& getAccount();
    
    // Virtual methods
    virtual int getMaxBooks() const = 0;
    virtual int getMaxDays() const = 0;
    virtual double calculateFine(int daysOverdue) const = 0;
    
    // Serialization
    virtual std::string serialize() const;
    static User* deserialize(const std::string& data);
};

class Student : public User {
public:
    Student(const std::string& name, const std::string& email, 
            const std::string& password);
    
    int getMaxBooks() const override { return 3; }
    int getMaxDays() const override { return 15; }
    double calculateFine(int daysOverdue) const override;
};

class Faculty : public User {
public:
    Faculty(const std::string& name, const std::string& email, 
            const std::string& password);
    
    int getMaxBooks() const override { return 5; }
    int getMaxDays() const override { return 30; }
    double calculateFine(int daysOverdue) const override;
};

class Librarian : public User {
public:
    Librarian(const std::string& name, const std::string& email, 
              const std::string& password);
    
    int getMaxBooks() const override { return 0; }
    int getMaxDays() const override { return 0; }
    double calculateFine(int daysOverdue) const override;
};

#endif
