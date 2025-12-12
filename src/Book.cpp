#include "Book.h"
#include <iostream>
#include <stdexcept>
#include <ctime>

Book::Book(const std::string& title, const std::string& author, 
           int year, const std::string& isbn)
    : title(title), author(author), year(year), isbn(isbn),
      available(true), borrowedBy("") {
    
    if (year < 1450 || year > 2025) {
        throw std::invalid_argument("Invalid year");
    }
}

std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
int Book::getYear() const { return year; }
std::string Book::getIsbn() const { return isbn; }
bool Book::isAvailable() const { return available; }
std::string Book::getBorrowedBy() const { return borrowedBy; }

void Book::borrow(const std::string& userName) {
    if (!available) throw std::runtime_error("Книгу уже взяли");
    available = false;
    borrowedBy = userName;
}

void Book::returnBook() {
    if (available) throw std::runtime_error("Книгу не брали");
    available = true;
    borrowedBy = "";
}

void Book::display() const {
    std::cout << title << " by " << author 
              << " (" << year << ") - " 
              << (available ? "Доступна" : "Книгу взял:" + borrowedBy) 
              << std::endl;
}