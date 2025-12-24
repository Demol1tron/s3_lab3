// CREATE <имя> ARRAY/SET/LIST/DLIST...

#include <iostream>

#include "file_io.h"
#include "parser.h"
#include "storage.h"

void printUsage() {
  std::cout << "Использование:" << '\n';
  std::cout << "  ./dbms --file <имя_файла> --query '<команда>'" << '\n';
  std::cout << '\n';
  std::cout << "Уникальные команды:" << '\n';
  std::cout << "  COUNT" << '\n';
  std::cout << "  EXISTS <имя_структуры>" << '\n';
  std::cout << "  DELETE <имя_структуры>" << '\n';
  std::cout << '\n';
  std::cout << "Доступные структуры:" << '\n';
  std::cout << "  M - Массив, F - Односвязный список, L - Двусвязный список"
            << '\n';
  std::cout << "  S - Стек, Q - Очередь, T - Дерево" << '\n';
}

auto main(int argc, char* argv[]) -> int {
  Storage storage;
  std::string filename;
  std::string query;

  // парсинг аргументов в терминале
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "--file" && i + 1 < argc) {
      filename = argv[++i];

    } else if (arg == "--query" && i + 1 < argc) {
      query = argv[++i];
    } else if (arg == "--help" || arg == "-h") {
      printUsage();
      return 0;
    }
  }

  if (filename.empty()) {
    std::cout << "ОШИБКА: не указан файл данных" << '\n';
    printUsage();
    return -1;
  }

  // загрузка из файла
  if (loadFromFile(&storage, filename) != OperationRes::SUCCESS) {
    std::cout << "СОЗДАН НОВЫЙ ФАЙЛ ДАННЫХ: " << filename << '\n';
  } else {
    std::cout << "ДАННЫЕ ЗАГРУЖЕНЫ ИЗ ФАЙЛА: " << filename << '\n';
  }

  if (!query.empty()) {
    ParsedCommand cmd = parseQuery(query);
    std::string result;
    OperationRes res = executeCommand(&storage, cmd, result);

    switch (res) {
      case OperationRes::SUCCESS:
        // if (!result.empty())
        std::cout << result << '\n';
        break;
      case OperationRes::ERROR:
        std::cout << "ОШИБКА" << '\n';
        break;
      case OperationRes::NOT_FOUND:
        std::cout << "НЕ_НАЙДЕНО" << '\n';
        break;
      case OperationRes::EMPTY:
        std::cout << "ПУСТО" << '\n';
        break;
      case OperationRes::INDEX_OUT:
        std::cout << "ИНДЕКС_ВЫШЕЛ_ЗА_ПРЕДЕЛЫ" << '\n';
        break;
    }

    saveToFile(&storage, filename);
    std::cout << "ДАННЫЕ ЗАГРУЖЕНЫ В ФАЙЛ: " << filename << '\n';
  } else {
    std::cout << "ОШИБКА: неправильно указана команда" << '\n';
    printUsage();
    return -1;
  }

  // очистка памяти
  storageCleanup(&storage);

  return 0;
}