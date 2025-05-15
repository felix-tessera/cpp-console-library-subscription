#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Transaction {
    string date;
    string time;
    string type;
    string account;
    double amount;
    string purpose;
};

// Функция для чтения данных из файла
vector<Transaction> readTransactions(const string& filename) {
    vector<Transaction> transactions;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Не удалось открыть файл: " << filename << endl;
        return transactions;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        Transaction t;

        iss >> t.date >> t.time >> t.type >> t.account >> t.amount;
        getline(iss, t.purpose); // Читаем оставшуюся часть строки как назначение

        // Удаляем лишний пробел в начале назначения
        if (!t.purpose.empty() && t.purpose[0] == ' ') {
            t.purpose = t.purpose.substr(1);
        }

        transactions.push_back(t);
    }

    file.close();
    return transactions;
}

// Функция для фильтрации приходных операций
vector<Transaction> filterIncome(const vector<Transaction>& transactions) {
    vector<Transaction> result;
    for (const auto& t : transactions) {
        if (t.type == "приход") {
            result.push_back(t);
        }
    }
    return result;
}

// Функция для фильтрации операций за ноябрь 2021 года
vector<Transaction> filterNovember2021(const vector<Transaction>& transactions) {
    vector<Transaction> result;
    for (const auto& t : transactions) {
        if (t.date.substr(3, 2) == "11" && t.date.substr(6, 2) == "21") {
            result.push_back(t);
        }
    }
    return result;
}

// Сортировка выбором по назначению платежа
void selectionSortByPurpose(vector<Transaction>& transactions) {
    for (size_t i = 0; i < transactions.size() - 1; ++i) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < transactions.size(); ++j) {
            if (transactions[j].purpose < transactions[min_idx].purpose) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap(transactions[i], transactions[min_idx]);
        }
    }
}

// Вспомогательная функция для быстрой сортировки
int partition(vector<Transaction>& transactions, int low, int high) {
    Transaction pivot = transactions[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (transactions[j].account < pivot.account ||
            (transactions[j].account == pivot.account && transactions[j].type < pivot.type) ||
            (transactions[j].account == pivot.account && transactions[j].type == pivot.type && transactions[j].amount > pivot.amount)) {
            i++;
            swap(transactions[i], transactions[j]);
        }
    }

    swap(transactions[i + 1], transactions[high]);
    return i + 1;
}

// Быстрая сортировка по счету, виду операции и сумме
void quickSort(vector<Transaction>& transactions, int low, int high) {
    if (low < high) {
        int pi = partition(transactions, low, high);
        quickSort(transactions, low, pi - 1);
        quickSort(transactions, pi + 1, high);
    }
}

// Функция для вывода транзакций
void printTransactions(const vector<Transaction>& transactions) {
    for (const auto& t : transactions) {
        cout << t.date << " " << t.time << " " << t.type << " "
            << t.account << " " << fixed << setprecision(2) << t.amount
            << " " << t.purpose << endl;
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    // Чтение данных из файла
    vector<Transaction> transactions = readTransactions("data.txt");

    // 1. Вывести все приходные операции
    cout << "Приходные операции:" << endl;
    vector<Transaction> income = filterIncome(transactions);
    printTransactions(income);
    cout << endl;

    // 2. Вывести операции за ноябрь 2021
    cout << "Операции за ноябрь 2021:" << endl;
    vector<Transaction> november2021 = filterNovember2021(transactions);
    printTransactions(november2021);
    cout << endl;

    // 3. Сортировка выбором по назначению платежа
    cout << "Сортировка выбором по назначению платежа:" << endl;
    vector<Transaction> sortedByPurpose = transactions;
    selectionSortByPurpose(sortedByPurpose);
    printTransactions(sortedByPurpose);
    cout << endl;

    // 4. Быстрая сортировка по счету, виду операции и сумме
    cout << "Быстрая сортировка по счету, виду операции и сумме:" << endl;
    vector<Transaction> quickSorted = transactions;
    quickSort(quickSorted, 0, quickSorted.size() - 1);
    printTransactions(quickSorted);

    return 0;
}