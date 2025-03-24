#pragma once

#include <mutex>

class Data
{
private:

	std::mutex m;
	int data{};

public:

	Data(int value);	// Конструктор

	void setData(int value);	// Сеттер

	int getData() const;	// Геттер

	// Дружественные функции - имеют доступ к мьютексу
	friend void swap_lock(Data& data1, Data& data2);
	friend void swap_unique_lock(Data& data1, Data& data2);
	friend void swap_scoped_lock(Data& data1, Data& data2);
};

