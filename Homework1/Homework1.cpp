/*
Очередь клиентов
Вам нужно создать приложение, которое имитирует очередь в окошко. Для этого нужно создать два потока, работающие с одной разделяемой переменной.

Первый поток имитирует клиента: раз в секунду он обращается к счётчику клиентов и увеличивает его на 1. Максимальное количество клиентов должно быть параметризировано.

Второй поток имитирует операциониста: раз в 2 секунды он обращается к счётчику клиентов и уменьшает его на 1. «Операционист» работает до последнего клиента.

Нужно модифицировать
задание 1 к первому уроку
так, чтобы счётчик клиентовбыл атомарным.
Все операции со счётчиков должны быть атомарными.
Проверьте работу различными способами упорядочения доступа к памяти.
*/

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>

std::mutex m;

/// <summary>
/// Добавление клиента в очередь
/// </summary>
/// <param name="value">Полное количество клиентов</param>
/// <param name="count">Ссылка на счётчик</param>
void pushToQueue(int value, std::atomic<int>& count)
{
	std::unique_lock<std::mutex> lg(m);
	lg.unlock();
	for (size_t i{}; i < value; ++i)
	{
		//lg.lock();
		lg.lock();
		count.store(count.load() + 1);	// атомарные функции
		std::cout << "Client added.   Queue includes " << count.load() << " clients.\n";
		lg.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

/// <summary>
/// Удаление клиента из очереди
/// </summary>
/// <param name="count">Ссылка на счётчик</param>
void popFromQueue(std::atomic<int>& count)
{
	std::unique_lock<std::mutex> lg(m);
	lg.unlock();
	while (count > 0)
	{
		//lg.lock();
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		lg.lock();
		//count.store(count.load() - 1);
		std::cout << "Client deleted. Queue includes " << (--count) << " clients.\n";
		lg.unlock();
	}
}

int main()
{
	std::atomic<int> count{};	// Атомарный счётчик
	int value{ 20 };	// Количество клиентов

	std::thread threadPush(pushToQueue, value, std::ref(count));	// Каждые 1000 мс один клиент добавляется в очередь
	std::this_thread::sleep_for(std::chrono::milliseconds(1500));	// С этим временным промежутком изменения в очереди выглядят красивее. Необязательная команда.
	std::thread threadPop(popFromQueue, std::ref(count));			// Каждые 2000 мс операционист обслуживает одного клиента
	threadPush.join();	// Завершение потока
	threadPop.join();	// Завершение потока

	return EXIT_SUCCESS;
}

