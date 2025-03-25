// Homework3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <mutex>
#include "Data.h"

// Прототипы функций
void printData(const Data& data1, const Data& data2);

void swap_lock(Data& data1, Data& data2);
void swap_unique_lock(Data& data1, Data& data2);
void swap_scoped_lock(Data& data1, Data& data2);

int main()
{
    Data data1(10);
    Data data2(20);
    printData(data1, data2);

    // Вызовы разных swap
    swap_lock(data1, data2);
    //swap_unique_lock(data1, data2);
    //swap_scoped_lock(data1, data2);

    std::cout << "Swaped!\n";
    printData(data1, data2);

    return EXIT_SUCCESS;
}

/// <summary>
/// Вывод в консоль информации об объектах
/// </summary>
/// <param name="data1"></param>
/// <param name="data2"></param>
void printData(const Data& data1, const Data& data2)
{
    std::cout << "data1 = " << data1.getData() << "; data2 = " << data2.getData() << std::endl;
}

/// <summary>
/// Простой swap без доступа к мьютексу - вызывается во всех дружественных swap
/// </summary>
/// <param name="data1">Первый объект</param>
/// <param name="data2">Второй объект</param>
void swap(Data& data1, Data& data2)
{
    int temp{ data1.getData() };
    data1.setData(data2.getData());
    data2.setData(temp);
}

/// <summary>
/// swap с использованием метода lock
/// </summary>
/// <param name="data1">Первый объект</param>
/// <param name="data2">Второй объект</param>
void swap_lock(Data& data1, Data& data2)
{
    std::lock(data1.m, data2.m);
    std::lock_guard<std::mutex> lg1{ data1.m, std::adopt_lock };
    std::lock_guard<std::mutex> lg2{ data2.m, std::adopt_lock };
    swap(data1, data2);
}

/// <summary>
/// swap с исполльзованием unique_lock
/// </summary>
/// <param name="data1">Первый объект</param>
/// <param name="data2">Второй объект</param>
void swap_unique_lock(Data& data1, Data& data2)
{
    std::unique_lock<std::mutex> ul1{ data1.m, std::defer_lock };
    std::unique_lock<std::mutex> ul2{ data2.m, std::defer_lock };
    swap(data1, data2);
}

/// <summary>
/// swap с исполльзованием scoped_lock
/// </summary>
/// <param name="data1">Первый объект</param>
/// <param name="data2">Второй объект</param>
void swap_scoped_lock(Data& data1, Data& data2)
{
    std::scoped_lock sl{ data1.m, data2.m };
    swap(data1, data2);
}
