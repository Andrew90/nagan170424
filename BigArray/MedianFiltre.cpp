#include "MedianFiltre.h"
#include <memory>

double MedianFiltre::Do(double data)
{
	buff[current % width] = data; 

	int counters[size] = {};

	for (unsigned i = 0; i < width - 1; ++i)
	{
		for (unsigned j = i + 1; j < width; ++j)
		{
			if (buff[i] >= buff[j]) ++counters[i];
			else ++counters[j];
		}
	}
	double res = 0;
	for (unsigned i = 0; i < width; ++i)
	{
		if (middle == counters[i])
		{
			res = buff[i];
			break;
		}
	}	
	++current;
	return res;
}

void MedianFiltre::SetWidth(int w)
{
	width = w;
	middle = w / 2;
	current = w - 1;
	for(int i = 0; i < w; ++i) buff[i] = 0;
}
