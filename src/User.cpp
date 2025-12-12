#include "User.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

User::User(const std::string& name, const std::string& userId)
    : name(name), userId(userId), maxBooksAllowed(3) {}

std::string User::getName() const { return name; }
std::string User::getUserId() const { return userId; }
std::vector<std::string> User::getBorrowedBooks() const { return borrowedBooks; }
int User::getMaxBooksAllowed() const { return maxBooksAllowed; }

bool User::canBorrowMore() const {
    return borrowedBooks.size() < (size_t)maxBooksAllowed;
}

void User::addBook(const std::string& isbn) {
    if (!canBorrowMore()) {
        throw std::runtime_error("Достигнут лимит книг");
    }
    borrowedBooks.push_back(isbn);
}

void User::removeBook(const std::string& isbn) {
    auto it = std::find(borrowedBooks.begin(), borrowedBooks.end(), isbn);
    if (it == borrowedBooks.end()) {
        throw std::runtime_error("Книги нет у пользователя");
    }
    borrowedBooks.erase(it);
}

void User::displayProfile() const {
    std::cout << "\nПользователь: " << name << std::endl;
    std::cout << "ID: " << userId << std::endl;
    std::cout << "Книг взято: " << borrowedBooks.size() 
              << " из " << maxBooksAllowed << std::endl;
}