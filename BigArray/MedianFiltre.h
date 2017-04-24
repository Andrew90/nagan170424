#pragma once
class MedianFiltre
{
	static const int size = 32;
	unsigned width;
	unsigned current;
	unsigned middle;
	double buff[size];
	//int counters[size];
public:
	double Do(double);
	void SetWidth(int);
};

