#include "file_reader.h"
#include "constants.h"

#include <fstream>
#include <cstring>

operation convert(char* str)
{
    date result;
    char* context = NULL;
    char* str_number = strtok_s(str, ".", &context);
    result.day = atoi(str_number);
    str_number = strtok_s(NULL, ".", &context);
    result.month = atoi(str_number);
    str_number = strtok_s(NULL, ".", &context);
    result.year = atoi(str_number);
    return result;
}

void read(const char* file_name, operation* array[], int& size)
{
    std::ifstream file(file_name);
    if (file.is_open())
    {
        size = 0;
        char tmp_buffer[MAX_STRING_SIZE];
        while (!file.eof())
        {
            operation* item = new operation;
            file >> item->reader.date;
            file >> item->reader.time;
            file >> item->reader.type;
            file >> item->reader.account;
            file >> item->reader.sum;
            file >> item->reader.description;
            file >> tmp_buffer;
            item->start = convert(tmp_buffer);
            file >> tmp_buffer;
            item->finish = convert(tmp_buffer);
            file >> item->reader.date;
            file >> item->reader.time;
            file >> item->reader.type;
            file >> item->reader.account;
            file >> item->reader.sum;
            file >> item->reader.description;
            file.read(tmp_buffer, 1); // чтения лишнего символа пробела
            file.getline(item->title, MAX_STRING_SIZE);
            array[size++] = item;
        }
        file.close();
    }
    else
    {
        throw "Ошибка открытия файла";
    }
}