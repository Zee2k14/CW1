#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>

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

// Librarian class derived from Person
class Librarian : public Person {
private:
    int staffID;
    float salary;

public:
    Librarian(int id, const std::string& name, const std::string& address, const std::string& email, float salary)
        : Person(name, address, email), staffID(id), salary(salary) {}
    void displayBorrowedBooks(const Member& member) const {
    const auto& booksLoaned = member.getBooksBorrowed();
    if (booksLoaned.empty()) {
        std::cout << "No books borrowed by member ID " << member.getMemberID() << "." << std::endl;
    } else {
        std::cout << "Books borrowed by member ID " << member.getMemberID() << ":" << std::endl;
        for (const auto* book : booksLoaned) {
            std::cout << "Book ID: " << book->getBookID()
                      << ", Title: " << book->getBookName()
                      << ", Author: " << book->getAuthorFirstName() << " " << book->getAuthorLastName()
                      << ", Due Date: ";
            // Convert due date to string and print
            char dueDateStr[11];
            std::tm dueDate = book->getDueDate(); // Store the due date in a non-temporary variable
            std::strftime(dueDateStr, sizeof(dueDateStr), "%Y-%m-%d", &dueDate);
            std::cout << dueDateStr << std::endl;
        }
    }
}

    void addMember(std::vector<Member>& members, int id, const std::string& name, const std::string& address, const std::string& email) {
        members.emplace_back(id, name, address, email);
    }

void issueBook(std::vector<Book>& books, Member* member, int bookID) {
    for (auto& book : books) {
        if (book.getBookID() == bookID && book.getBorrower() == nullptr) {
            std::tm issueDate = {}; // Initialize to zero
            std::cout << "Enter issue date for the book (YYYY MM DD): ";
            std::cin >> std::get_time(&issueDate, "%Y %m %d");
            mktime(&issueDate); // Normalize the tm structure

            // Set due date to 3 days from issue date
            std::tm dueDate = issueDate;
            dueDate.tm_mday += 3; // Add 3 days for the due date
            mktime(&dueDate); // Normalize the due date
            
            book.borrowBook(member, dueDate);
            std::cout << "Book issued successfully. Due date: ";
            char dueDateStr[11];
            std::strftime(dueDateStr, sizeof(dueDateStr), "%Y-%m-%d", &dueDate);
            std::cout << dueDateStr << std::endl;
            break;
        }
    }
}

void returnBook(std::vector<Book>& books, Member* member, int bookID) {
    for (auto& book : books) {
        if (book.getBookID() == bookID && book.getBorrower() == member) {
            std::tm returnDate = {}; // Initialize to zero
            std::cout << "Enter return date for the book (YYYY MM DD): ";
            std::cin >> std::get_time(&returnDate, "%Y %m %d");
            mktime(&returnDate); // Normalize the tm structure

            // Now calculate fine based on the return date and the due date
            float fine = calcFine(book, returnDate);
            book.returnBook();
            if (fine > 0) {
               std::cout << "Book returned. Fine due: Pounds " << fine << std::endl;

            } else {
                std::cout << "Book returned. No fine due." << std::endl;
            }
            break;
        }
    }
}

float calcFine(const Book& book, const std::tm& returnDate) const {
    std::tm dueDate = book.getDueDate();

    // Convert both dates to time_t for comparison
    std::time_t due = std::mktime(const_cast<std::tm*>(&dueDate));
    std::time_t returned = std::mktime(const_cast<std::tm*>(&returnDate));

    // Calculate difference in seconds
    double seconds = std::difftime(returned, due);
    int daysOverdue = static_cast<int>(seconds / (60 * 60 * 24));

    if (daysOverdue > 0) {
        return static_cast<float>(daysOverdue); // £1 per day
    }
    return 0.0f;
}


    int getStaffID() const { return staffID; }
    float getSalary() const { return salary; }

    void setStaffID(int newID) { staffID = newID; }
    void setSalary(float newSalary) { salary = newSalary; }
};