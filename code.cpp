#include <iostream>
#include <string>
#include <vector>

// Base class Person
class Person {
protected:
    std::string name;
    std::string address;
    std::string email;

public:
    Person(const std::string& name, const std::string& address, const std::string& email)
        : name(name), address(address), email(email) {}

    std::string getName() const { return name; }
    std::string getAddress() const { return address; }
    std::string getEmail() const { return email; }

    void setName(const std::string& newName) { name = newName; }
    void setAddress(const std::string& newAddress) { address = newAddress; }
    void setEmail(const std::string& newEmail) { email = newEmail; }
};

// Forward declaration
class Book;

// Member class derived from Person
class Member : public Person {
private:
    int memberID;
    std::vector<Book*> booksLoaned;

public:
    Member(int id, const std::string& name, const std::string& address, const std::string& email)
        : Person(name, address, email), memberID(id) {}

    int getMemberID() const { return memberID; }
    std::vector<Book*> getBooksBorrowed() const { return booksLoaned; }
    void addBookBorrowed(Book* book) { booksLoaned.push_back(book); }
    void removeBookBorrowed(Book* book) {
        booksLoaned.erase(std::remove(booksLoaned.begin(), booksLoaned.end(), book), booksLoaned.end());
    }
};

// Book class
class Book {
private:
    int bookID;
    std::string bookName;
    int pageCount; // Add page count
    std::string authorFirstName;
    std::string authorLastName;
    std::string bookType; // Add book type
    std::tm dueDate;
    Member* borrower;

public:
    Book(int id, const std::string& name, int pageCount, const std::string& authorFirst, 
         const std::string& authorLast, const std::string& type) // Update constructor
        : bookID(id), bookName(name), pageCount(pageCount), 
          authorFirstName(authorFirst), authorLastName(authorLast), 
          bookType(type), borrower(nullptr) {}

    int getBookID() const { return bookID; }
    std::string getBookName() const { return bookName; }
    std::string getAuthorFirstName() const { return authorFirstName; }
    std::string getAuthorLastName() const { return authorLastName; }
    std::tm getDueDate() const { return dueDate; }
    void setDueDate(const std::tm& newDueDate) { dueDate = newDueDate; }
    Member* getBorrower() const { return borrower; }
    void borrowBook(Member* newBorrower, const std::tm& newDueDate) {
        borrower = newBorrower;
        dueDate = newDueDate;
        borrower->addBookBorrowed(this);
    }

    void returnBook() {
        if (borrower != nullptr) {
            borrower->removeBookBorrowed(this);
            borrower = nullptr;
        }
    }
    int getPageCount() const { return pageCount; }
    std::string getBookType() const { return bookType; }
};