// Homework2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <ctime>
#include <Windows.h>
#include <exception>

//#define EXCETPION_IMITATION	// Раскомментировать для имитации исключения

#define ARRAY_SIZE 5	// Размер массива - количество прогресс-баров

const char FILL_BAR = static_cast<char>(178);	// Символ заполнения шкалы прогресс-бара
constexpr uint8_t BAR_LENGTH{ 25 };				// Длина прогресс-бара
constexpr uint8_t START_POSITION{ 10 };			// Стартовая позиция прогресс-бара
constexpr unsigned int MAX_ID{ 100'000 };		// Максимально возможный ID процесса

HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);	// Объект для смены позиции курсора в консоли

std::mutex m;

// Прототипы функций
void printHead();
void printBar(unsigned int raw, unsigned int bar_id);
void goto_xy(int x, int y);

int main()
{
	try
	{

#ifdef EXCETPION_IMITATION
		SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
		srand(static_cast<unsigned int>(time(NULL)));
#endif // EXCETPION_IMITATION

		unsigned int id[ARRAY_SIZE]{};	// массив ID для прогресс-баров
		std::thread myThreads[ARRAY_SIZE]{};	// Массив тредов для многопоточного заполнения прогресс-баров

		// Генерация рандомных ID
		for (size_t i = 0; i < ARRAY_SIZE; ++i)
		{
			id[i] = 1 + rand() % MAX_ID;
		}

		printHead();	// Печать шапки таблицы

		// Разбиение на потоки
		std::thread myThead[ARRAY_SIZE];

		for (unsigned int i = 0; i < ARRAY_SIZE; ++i)
		{
			myThead[i] = std::thread(printBar, i + 1, id[i]);
		}

		for (unsigned int i = 0; i < ARRAY_SIZE; ++i)
		{
			myThead[i].join();
		}

		// Смена позиции курсора
		goto_xy(0, ARRAY_SIZE + 1);
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

    return EXIT_SUCCESS;
}

/// <summary>
/// Смена позиции курсора
/// </summary>
/// <param name="x">Координата Х</param>
/// <param name="y">Координата У</param>
void goto_xy(int x, int y)
{
	COORD position;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(hConsole, position);
}

/// <summary>
/// Заполнение прогресс-бара
/// </summary>
/// <param name="raw">Номер строки</param>
/// <param name="bar_id">ID</param>
void printBar(unsigned int raw, unsigned int bar_id)
{
	//srand(static_cast<unsigned int>(time(NULL)));
	std::unique_lock<std::mutex>ul{ m };
	goto_xy(0, raw);
	std::cout << raw << ' ' << bar_id << ' ';
	ul.unlock();
	std::chrono::steady_clock::time_point startTime{ std::chrono::steady_clock::now() };

	for (unsigned int i = 0; i < BAR_LENGTH; ++i)
	{
		ul.lock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		goto_xy(START_POSITION + i, raw);

#ifdef EXCETPION_IMITATION

		int randomInt = rand() % 10;
		if (randomInt == 9)
		{
			//system("color 04");

			SetConsoleTextAttribute(handle, FOREGROUND_RED);
			std::cout << "\tException imitation!";
			SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
			//system("color 07");
			ul.unlock();
			return;
		}

#endif // EXCETPION_IMITATION

		std::cout << FILL_BAR;
		ul.unlock();
		int randInt{ 1 + rand() % 10 };
	}

	std::chrono::steady_clock::time_point finishTime{ std::chrono::steady_clock::now() };
	std::chrono::duration<double, std::milli> period{ finishTime - startTime };

	ul.lock();
	goto_xy(37, raw);
	std::cout << period.count() << " ms";
	ul.unlock();
}

/// <summary>
/// Печать шапки таблицы
/// </summary>
void printHead()
{
	std::cout << '#';

	goto_xy(2, 0);
	std::cout << "id";

	goto_xy(10, 0);
	std::cout << "Progress bar";

	goto_xy(37, 0);
	std::cout << "Time\n";
}

