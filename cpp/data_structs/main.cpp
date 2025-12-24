#include <iostream>
#include "storage.h"
#include "parser.h"
#include "file_io.h"

void printUsage()
{
    std::cout << "Использование:" << std::endl;
    std::cout << "  ./dbms --file <имя_файла> --query '<команда>'" << std::endl;
    std::cout << std::endl;
    std::cout << "Уникальные команды:" << std::endl;
    std::cout << "  COUNT" << std::endl;
    std::cout << "  EXISTS <имя_структуры>" << std::endl;
    std::cout << "  DELETE <имя_структуры>" << std::endl;
    std::cout << std::endl;
    std::cout << "Доступные структуры:" << std::endl;
    std::cout << "  M - Массив, F - Односвязный список, L - Двусвязный список" << std::endl;
    std::cout << "  S - Стек, Q - Очередь, T - Дерево" << std::endl;
}

int main(int argc, char *argv[])
{
    Storage storage;
    std::string filename;
    std::string query;

    // парсинг аргументов в терминале
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--file" && i + 1 < argc)
            filename = argv[++i];
        
        else if (arg == "--query" && i + 1 < argc)
            query = argv[++i];
        else if (arg == "--help" || arg == "-h") {
            printUsage();
            return 0;
        }
    }
    
    if (filename.empty()) {
        std::cout << "ОШИБКА: не указан файл данных" << std::endl;
        printUsage();
        return -1;
    }
    
    // загрузка из файла
    if (loadFromFile(&storage, filename) != OperationRes::SUCCESS)
        std::cout << "СОЗДАН НОВЫЙ ФАЙЛ ДАННЫХ: " << filename << std::endl;
    else
        std::cout << "ДАННЫЕ ЗАГРУЖЕНЫ ИЗ ФАЙЛА: " << filename << std::endl;
    
    if (!query.empty()) {
        ParsedCommand cmd = parseQuery(query);
        std::string result;
        OperationRes res = executeCommand(&storage, cmd, result);
        
        switch (res)
        {
            case OperationRes::SUCCESS:
                // if (!result.empty())
                std::cout << result << std::endl;
                break;
            case OperationRes::ERROR:
                std::cout << "ОШИБКА" << std::endl;
                break;
            case OperationRes::NOT_FOUND:
                std::cout << "НЕ_НАЙДЕНО" << std::endl;
                break;
            case OperationRes::EMPTY:
                std::cout << "ПУСТО" << std::endl;
                break;
            case OperationRes::INDEX_OUT:
                std::cout << "ИНДЕКС_ВЫШЕЛ_ЗА_ПРЕДЕЛЫ" << std::endl;
                break;
        }
        
        saveToFile(&storage, filename);
        std::cout << "ДАННЫЕ ЗАГРУЖЕНЫ В ФАЙЛ: " << filename << std::endl;
    } else {
        std::cout << "ОШИБКА: неправильно указана команда" << std::endl;
        printUsage();
        return -1;
    }
    
    // очистка памяти
    storageCleanup(&storage);
    
    return 0;
}