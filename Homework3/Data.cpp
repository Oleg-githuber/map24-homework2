#include "Data.h"

Data::Data(int value) : data{ value }
{}

void Data::setData(int value)
{
	this->data = value;
}

int Data::getData() const
{
	return this->data;
}