#include <iostream>
#include <string>
#include <vector>
#include <memory>

// ============================================================
// Базовый класс
// ============================================================
class Employee {
protected:
    std::string name;

public:
    explicit Employee(std::string n) : name(std::move(n)) {}

    // Виртуальный деструктор — обязателен при наличии виртуальных методов
    virtual ~Employee() = default;

    // Виртуальный метод, который будут переопределять наследники
    virtual void work() const {
        std::cout << "[Base] Сотрудник " << name
                  << " выполняет общие обязанности.\n";
    }

    const std::string& getName() const { return name; }
};

// ============================================================
// Производный класс 1: Разработчик
// ============================================================
class Developer : public Employee {
private:
    std::string language;

public:
    Developer(std::string n, std::string lang)
        : Employee(std::move(n)), language(std::move(lang)) {}

    // Переопределение метода базового класса
    void work() const override {
        // 1. Вызов метода БАЗОВОГО класса
        Employee::work();
        // 2. Добавляем свою специфику
        std::cout << "[Derived-Dev] ...а именно пишет код на "
                  << language << ".\n";
    }

    // Уникальный метод производного класса
    void writeCode() const {
        std::cout << "[Derived-Dev] " << name
                  << " фиксит баги в продакшене!\n";
    }
};

// ============================================================
// Производный класс 2: Менеджер
// ============================================================
class Manager : public Employee {
public:
    explicit Manager(std::string n) : Employee(std::move(n)) {}

    void work() const override {
        Employee::work();
        std::cout << "[Derived-Manager] ...а именно проводит совещания "
                     "и ставит задачи команде.\n";
    }

    void holdMeeting() const {
        std::cout << "[Derived-Manager] " << name
                  << " созывает всех на дейли-митинг.\n";
    }
};

// ============================================================
// Вспомогательная функция для безопасного чтения строки
// ============================================================
std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

// ============================================================
// main
// ============================================================
int main() {
    // Устанавливаем UTF-8 для корректного вывода кириллицы (для Windows)
    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif

    std::cout << "=== Демонстрация наследования и полиморфизма ===\n\n";

    // 1. Ввод данных с клавиатуры
    std::string devName = readLine("Введите имя разработчика: ");
    std::string devLang = readLine("Введите язык программирования разработчика: ");
    std::string mgrName = readLine("Введите имя менеджера: ");

    std::cout << "\n--- Создаём объекты и помещаем их в контейнер "
                 "указателей на базовый класс ---\n\n";

    // 2. Создаём объекты производных классов
    std::vector<std::unique_ptr<Employee>> team;
    team.push_back(std::make_unique<Developer>(devName, devLang));
    team.push_back(std::make_unique<Manager>(mgrName));

    // 3. Демонстрация полиморфизма:
    //    вызываем work() через указатель на базовый класс,
    //    но выполняется код производного класса
    std::cout << "--- Полиморфный вызов метода work() ---\n";
    for (const auto& emp : team) {
        emp->work();
        std::cout << "------------------------\n";
    }

    // 4. Демонстрация вызова уникальных методов производных классов
    //    через приведение типов
    std::cout << "\n--- Вызов уникальных методов производных классов ---\n";

    if (auto* dev = dynamic_cast<Developer*>(team[0].get())) {
        dev->writeCode();
    }
    if (auto* mgr = dynamic_cast<Manager*>(team[1].get())) {
        mgr->holdMeeting();
    }

    return 0;
}
