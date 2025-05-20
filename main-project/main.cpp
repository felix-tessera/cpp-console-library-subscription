#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#include "operation_subscription.h"
#include "file_reader.h"
#include "constants.h"

// данные об операции
struct Operation {
    string date;
    string time;
    string type;
    string account;
    double sum;
    string description;
};

// чтеник файла
vector<Operation> readFile(string filename) {
    vector<Operation> ops;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        Operation op;
        int space1 = line.find(' ');
        int space2 = line.find(' ', space1 + 1);
        int space3 = line.find(' ', space2 + 1);
        int space4 = line.find(' ', space3 + 1);

        op.date = line.substr(0, space1);
        op.time = line.substr(space1 + 1, space2 - space1 - 1);
        op.type = line.substr(space2 + 1, space3 - space2 - 1);
        op.account = line.substr(space3 + 1, space4 - space3 - 1);

        //начало суммы
        int sumStart = space4 + 1;
        int sumEnd = line.find(' ', sumStart);

        // если сумма последнее поле
        if (sumEnd == string::npos) {
            op.sum = stod(line.substr(sumStart));
            op.description = "";
        }
        else {
            op.sum = stod(line.substr(sumStart, sumEnd - sumStart));
            op.description = line.substr(sumEnd + 1);
        }

        ops.push_back(op);
    }

    return ops;
}

// вывод операций
void printOps(vector<Operation> ops) {
    for (auto op : ops) {
        cout << op.date << " " << op.time << " " << op.type << " "
            << op.account << " " << op.sum << " " << op.description << endl;
    }
}

//приходы
vector<Operation> getPrihods(vector<Operation> ops) {
    vector<Operation> result;
    for (auto op : ops) {
        if (op.type == "приход") {
            result.push_back(op);
        }
    }
    return result;
}

//операции за ноябрь 2021
vector<Operation> getNovember2021(vector<Operation> ops) {
    vector<Operation> result;
    for (auto op : ops) {
        // проверяем дату (формат дд.мм.гг)
        if (op.date.substr(3, 2) == "11" && op.date.substr(6, 2) == "21") {
            result.push_back(op);
        }
    }
    return result;
}

//сортировка выбором по описанию
void sortByDescription(vector<Operation>& ops) {
    for (int i = 0; i < ops.size() - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < ops.size(); j++) {
            if (ops[j].description < ops[minIndex].description) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            swap(ops[i], ops[minIndex]);
        }
    }
}

// быстрая сортировка
void quickSort(vector<Operation>& ops, int left, int right) {
    if (left >= right) return;

    Operation pivot = ops[(left + right) / 2];
    int i = left, j = right;

    while (i <= j) {
        while (true) {
            if (ops[i].account < pivot.account) break;
            if (ops[i].account == pivot.account && ops[i].type < pivot.type) break;
            if (ops[i].account == pivot.account && ops[i].type == pivot.type && ops[i].sum > pivot.sum) break;
            i++;
        }

        while (true) {
            if (ops[j].account > pivot.account) break;
            if (ops[j].account == pivot.account && ops[j].type > pivot.type) break;
            if (ops[j].account == pivot.account && ops[j].type == pivot.type && ops[j].sum < pivot.sum) break;
            j--;
        }

        if (i <= j) {
            swap(ops[i], ops[j]);
            i++;
            j--;
        }
    }

    quickSort(ops, left, j);
    quickSort(ops, i, right);
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    cout << "Автор: Ермолюгин Никита Сергеевич. Вариант: 7; Банковские операции \n";
    cout << "Введите имя файла: ";
    string filename;
    cin >> filename;

    vector<Operation> operations = readFile(filename);

    cout << "\nПриходные операции" << endl;
    vector<Operation> prihods = getPrihods(operations);
    printOps(prihods);

    cout << "\nОперации за ноябрь 2021" << endl;
    vector<Operation> november = getNovember2021(operations);
    printOps(november);

    cout << "\nСортировка по описанию" << endl;
    vector<Operation> ops1 = operations;
    sortByDescription(ops1);
    printOps(ops1);

    cout << "\nБыстрая сортировка" << endl;
    vector<Operation> ops2 = operations;
    quickSort(ops2, 0, ops2.size() - 1);
    printOps(ops2);

    return 0;
}
