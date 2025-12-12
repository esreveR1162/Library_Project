#include <iostream>
#include <string>
#include <limits>
#include "Library.h"

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void showMenu() {
    std::cout << "\n=== БИБЛИОТЕКА ===" << std::endl;
    std::cout << "1. Все книги" << std::endl;
    std::cout << "2. Все пользователи" << std::endl;
    std::cout << "3. Добавить книгу" << std::endl;
    std::cout << "4. Добавить пользователя" << std::endl;
    std::cout << "5. Выдать книгу" << std::endl;
    std::cout << "6. Вернуть книгу" << std::endl;
    std::cout << "7. Найти книгу по ISBN" << std::endl;
    std::cout << "8. Профиль пользователя" << std::endl;
    std::cout << "9. Сохранить" << std::endl;
    std::cout << "10. Выход" << std::endl;
    std::cout << "Выбор: ";
}

int main() {
    Library lib;
    
    bool running = true;
    while (running) {
        showMenu();
        
        int option;
        std::cin >> option;
        clearInput();
        
        try {
            switch (option) {
                case 1:
                    lib.displayAllBooks();
                    break;
                    
                case 2:
                    lib.displayAllUsers();
                    break;
                    
                case 3: {
                    std::string title, author, isbn;
                    int year;
                    
                    std::cout << "Название: ";
                    getline(std::cin, title);
                    
                    std::cout << "Автор: ";
                    getline(std::cin, author);
                    
                    std::cout << "Год: ";
                    std::cin >> year;
                    clearInput();
                    
                    std::cout << "ISBN: ";
                    getline(std::cin, isbn);
                    
                    Book newBook(title, author, year, isbn);
                    lib.addBook(newBook);
                    break;
                }
                    
                case 4: {
                    std::string name, id;
                    
                    std::cout << "Фамилия: ";
                    getline(std::cin, name);
                    
                    std::cout << "ID: ";
                    getline(std::cin, id);
                    
                    User newUser(name, id);
                    lib.addUser(newUser);
                    break;
                }
                    
                case 5: {
                    std::string name, isbn;
                    
                    std::cout << "Фамилия: ";
                    getline(std::cin, name);
                    
                    std::cout << "ISBN: ";
                    getline(std::cin, isbn);
                    
                    lib.borrowBook(name, isbn);
                    break;
                }
                    
                case 6: {
                    std::string isbn;
                    
                    std::cout << "ISBN: ";
                    getline(std::cin, isbn);
                    
                    lib.returnBook(isbn);
                    break;
                }
                    
                case 7: {
                    std::string isbn;
                    
                    std::cout << "ISBN: ";
                    getline(std::cin, isbn);
                    
                    Book* found = lib.findBookByISBN(isbn);
                    if (found) {
                        found->displayInfo();
                    } else {
                        std::cout << "Не найдено" << std::endl;
                    }
                    break;
                }
                    
                case 8: {
                    std::string name;
                    
                    std::cout << "Фамилия: ";
                    getline(std::cin, name);
                    
                    User* found = lib.findUserByName(name);
                    if (found) {
                        found->displayProfile();
                    } else {
                        std::cout << "Не найдено" << std::endl;
                    }
                    break;
                }
                    
                case 9:
                    lib.saveToFile();
                    break;
                    
                case 10:
                    running = false;
                    std::cout << "Выход" << std::endl;
                    break;
                    
                default:
                    std::cout << "Неверный выбор" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        }
    }
    
    return 0;
}