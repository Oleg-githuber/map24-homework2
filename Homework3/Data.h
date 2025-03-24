#pragma once

#include <mutex>

class Data
{
private:

	std::mutex m;
	int data{};

public:

	Data(int value);	// �����������

	void setData(int value);	// ������

	int getData() const;	// ������

	// ������������� ������� - ����� ������ � ��������
	friend void swap_lock(Data& data1, Data& data2);
	friend void swap_unique_lock(Data& data1, Data& data2);
	friend void swap_scoped_lock(Data& data1, Data& data2);
};

