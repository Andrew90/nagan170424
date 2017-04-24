#ifndef PrepareSG_h
#define  PrepareSG_h
#include <vector>
class PrepareSG
{
public:
	static const int count_sensors = 18;     //количество датчиков
	static const int buffer_size   = 65536; //размер буфера 791
	unsigned timeStart;
	unsigned timeStop;
	PrepareSG();
	~PrepareSG();
	std::vector<int >   PrepareSG::Do(unsigned long *Base, int len, unsigned *time);
	static PrepareSG &Instance();
};
#endif

