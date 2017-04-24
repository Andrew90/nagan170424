#pragma once
#include <vector>

using namespace std;
struct BigArray
{
static const int countSensors = 18;//поперечник + продольник + группа прочности
static const int noopSensors = 12;//поперечник
static const int prodSensors = 4; //продольник
static const int solidSensors = 2; //группа прочности
static void Prepare(int popSensors    //ѕоперечных датчиков
  , unsigned long *inp    //данные с платы 791
  , int sizeInp
  , vector<vector<int> >(&offsets) //смещение зон
  , vector<vector<vector<double> > >(&pop)//–азбитые данные по поперечнику
  , vector<vector<vector<double> > >(&prod) //разбитые данные по продольнику
  );
static void Prepare(int popSensors
, unsigned long *inp
, int sizeInp
, vector<int>(&offset)
, vector<vector<vector<double> > >(&pop)
);
static void TimeToArrayOffsets(
  unsigned *adcTime, int adcCount, int adcFrame
  , unsigned *lirTime, int lirCount
  , vector<int>(&offset)
  );
};

