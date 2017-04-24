#include "PrepareSG.h"

#include "DebugMess.h"

PrepareSG::PrepareSG()
{
}


PrepareSG::~PrepareSG()
{
}

std::vector<int >  PrepareSG::Do(unsigned long *Base, int len, unsigned *time)
//(unsigned(&data)[180000], unsigned(&time)[500], unsigned offs0, unsigned offs1)
{
	int k = 0;
	unsigned o0 = 0;
	int maxFrames = len / buffer_size;
	for (; k < 512 && k < maxFrames; ++k)
	{
		if (timeStart < time[k])
		{
			o0 = int(((double)(k - 1) + (timeStart - time[k - 1]) / (time[k] - time[k - 1])) * buffer_size / count_sensors);
			break;
		}
	}
	unsigned o1 = 0;
	for (; k < 512 && k < maxFrames; ++k)
	{
		if (timeStop < time[k])
		{
			o1 = int(((double)(k - 1) + (timeStop - time[k - 1]) / (time[k] - time[k - 1])) * buffer_size / count_sensors);
			break;
		}
	}
	if(o1 == 0) o1 = (k - 1) * buffer_size / count_sensors;
   //	dprint("%d %d\n", o0, o1);

	int offs = 10 * (o1 - o0) / 100;

	int start = o0 + offs;
	int stop =  o1 - offs;

	std::vector<int > res;
	res.resize((stop - start) * 2);

	int j = 0;
	for (int i = start; i < stop; ++i)
	{
		j = i * count_sensors;
		if( j % 2 == 0)
		{
		short t = Base[j - 2];
		res[i] = t;
		}
		else
		{
		short t = Base[j - 1];
		res[i + 1] = t;
        }
	}
	return res;
}

PrepareSG &PrepareSG::Instance()
{
	static PrepareSG x; return x;
}