#include "Library.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

Library::Library(const std::string& filePath) : dataFile(filePath) {
    std::cout << "\n === ИНИЦИАЛИЗАЦИЯ БИБЛИОТЕКИ ===" << std::endl;
    std::cout << "Файл данных: " << dataFile << std::endl;
    
    try {
        loadFromFile();
        std::cout << "Данные успешно загружены!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
        std::cout << "Создана новая пустая библиотека" << std::endl;
    }
    std::cout << "=============================\n" << std::endl;
}

void Library::addBook(const Book& book) {
    books.push_back(book);
    std::cout << "Книга добавлена" << std::endl;
}

void Library::addUser(const User& user) {
    users.push_back(user);
    std::cout << "Пользователь добавлен" << std::endl;
}

void Library::borrowBook(const std::string& userName, const std::string& isbn) {
    Book* book = findBookByISBN(isbn);
    User* user = findUserByName(userName);
    
    if (!book || !user) {
        throw std::runtime_error("Не найдено");
    }
    
    if (!book->getIsAvailable()) {
        throw std::runtime_error("Книга уже выдана");
    }
    
    if (!user->canBorrowMore()) {
        throw std::runtime_error("Лимит книг");
    }
    
    book->borrowBook(userName);
    user->addBook(isbn);
    std::cout << "Книга выдана" << std::endl;
}

void Library::returnBook(const std::string& isbn) {
    Book* book = findBookByISBN(isbn);
    
    if (!book) {
        throw std::runtime_error("Книга не найдена");
    }
    
    if (book->getIsAvailable()) {
        throw std::runtime_error("Книга уже в библиотеке");
    }
    
    std::string borrower = book->getBorrowedBy();
    User* user = findUserByName(borrower);
    
    if (user) {
        try {
            user->removeBook(isbn);
        } catch (...) {
        }
    }
    
    book->returnBook();
    std::cout << "Книга возвращена" << std::endl;
}

Book* Library::findBookByISBN(const std::string& isbn) {
    for (auto& book : books) {
        if (book.getISBN() == isbn) {
            return &book;
        }
    }
    return nullptr;
}

User* Library::findUserByName(const std::string& name) {
    for (auto& user : users) {
        if (user.getName() == name) {
            return &user;
        }
    }
    return nullptr;
}

void Library::displayAllBooks() const {
    std::cout << "\n=== Книги ===" << std::endl;
    if (books.empty()) {
        std::cout << "Нет книг" << std::endl;
    } else {
        for (const auto& book : books) {
            book.displayInfo();
        }
    }
}

void Library::displayAllUsers() const {
    std::cout << "\n=== Пользователи ===" << std::endl;
    if (users.empty()) {
        std::cout << "Нет пользователей" << std::endl;
    } else {
        for (const auto& user : users) {
            user.displayProfile();
        }
    }
}

void Library::saveToFile() const {
    std::ofstream file(dataFile);
    if (!file) {
        throw std::runtime_error("Не могу открыть файл");
    }
    
    for (const auto& book : books) {
        file << "BOOK\n";
        file << "Title: " << book.getTitle() << "\n";
        file << "Author: " << book.getAuthor() << "\n";
        file << "Year: " << book.getYear() << "\n";
        file << "ISBN: " << book.getISBN() << "\n";
        file << "Available: " << (book.getIsAvailable() ? "yes" : "no") << "\n";
        file << "BorrowedBy: " << book.getBorrowedBy() << "\n\n";
    }
    
    file << "---USERS---\n\n";
    
    for (const auto& user : users) {
        file << "USER\n";
        file << "Name: " << user.getName() << "\n";
        file << "UserID: " << user.getUserId() << "\n";
        
        auto books = user.getBorrowedBooks();
        file << "BorrowedBooks: ";
        for (size_t i = 0; i < books.size(); i++) {
            file << books[i];
            if (i != books.size() - 1) file << "|";
        }
        file << "\n";
        file << "MaxBooks: " << user.getMaxBooksAllowed() << "\n\n";
    }
    
    file.close();
    std::cout << "Данные сохранены" << std::endl;
}

void Library::loadFromFile() {
    std::ifstream file(dataFile);
    if (!file) {
        throw std::runtime_error("Файл не найден: " + dataFile);
    }
    
    std::cout << "📂 Загрузка данных из файла..." << std::endl;
    
    std::string line;
    bool readingBooks = true;
    int booksLoaded = 0;
    int usersLoaded = 0;
    
  
    std::string currentTitle, currentAuthor, currentISBN, currentBorrowedBy, currentAvailable;
    int currentYear = 0;
    bool parsingBook = false;
    
    while (getline(file, line)) {
 
        if (line.empty()) {
 
            if (parsingBook && !currentTitle.empty()) {
                try {
                    Book book(currentTitle, currentAuthor, currentYear, currentISBN);
                    if (currentAvailable == "no") {
                        book.borrowBook(currentBorrowedBy);
                    }
                    books.push_back(book);
                    booksLoaded++;
                } catch (const std::exception& e) {
                    std::cout << "Ошибка при загрузке книги: " << e.what() << std::endl;
                }
                

                currentTitle = currentAuthor = currentISBN = currentBorrowedBy = currentAvailable = "";
                currentYear = 0;
                parsingBook = false;
            }
            continue;
        }
        
   
        if (line == "---USERS---") {
            readingBooks = false;
            continue;
        }
        
        if (readingBooks) {

            if (line == "BOOK") {
                parsingBook = true;
                continue;
            }
            
            if (parsingBook) {
                if (line.find("Title: ") == 0) {
                    currentTitle = line.substr(7);
                } else if (line.find("Author: ") == 0) {
                    currentAuthor = line.substr(8);
                } else if (line.find("Year: ") == 0) {
                    currentYear = std::stoi(line.substr(6));
                } else if (line.find("ISBN: ") == 0) {
                    currentISBN = line.substr(6);
                } else if (line.find("Available: ") == 0) {
                    currentAvailable = line.substr(11);
                } else if (line.find("BorrowedBy: ") == 0) {
                    currentBorrowedBy = line.substr(12);
                }
            }
        } else {
   
            if (line == "USER") {
                usersLoaded++;
            }
        }
    }
    
    file.close();
    std::cout << "Загружено книг: " << booksLoaded << std::endl;
    std::cout << "Загружено пользователей: " << usersLoaded << std::endl;
}