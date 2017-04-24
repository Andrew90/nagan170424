#include "BigArray.h"
#include "Global.h"
#include "DebugMess.h"

#if 0
void BigArray::Prepare(int popSensors
, unsigned long *inp
, int sizeInp
, vector<vector<int> >(&offset)
, vector<vector<vector<double> > >(&pop)
, vector<vector<vector<double> > >(&prod)
)
{
	int offsPop = 0;//инициализация пореречного результата
	pop.clear();
	vector<vector<double> >pp(popSensors);
	pop.push_back(pp);
	vector<int> &popOffset = offset[0];
	int offsInpPop = popOffset[0] * popSensors;
	int offsPopMax = popOffset.size() - 1;


	int offsProd = 0;//продольный
	prod.clear();
	vector<vector<double> >pr(prodSensors);
	prod.push_back(pr);
	vector<int> &prodOffset = offset[1];
	int offsInpProd = prodOffset[0] * prodSensors;
	int offsProdMax = prodOffset.size() - 1;

	int __prodSensors = noopSensors + prodSensors;

	for (int i = 0; i < sizeInp; ++i)
	{
		int sensor = i % countSensors;
		if (sensor < popSensors)
		{
			if (offsPop < offsPopMax && offsInpPop / popSensors >= popOffset[offsPop + 1])
			{
				++offsPop;
				offsInpPop = popOffset[offsPop] * popSensors - 1;
				vector<vector<double > >p(popSensors);
				pop.push_back(p);
			}
			++offsInpPop;
			double t = 100.0 * (short)inp[i]/ Globals::level100;
			pop[offsPop][sensor].push_back(t);
		}
		else if (sensor < noopSensors)
		{
		}
		else if (sensor < __prodSensors)
		{
			if (offsProd < offsProdMax && offsInpProd / prodSensors >= prodOffset[offsProd + 1])
			{
				++offsProd;
				offsInpProd = prodOffset[offsProd] * prodSensors - 1;
				vector < vector<double> >p(prodSensors);
				prod.push_back(p);
			}
			++offsInpProd;
			double t = 100.0 * (short)inp[i]/ Globals::level100;
			prod[offsProd][sensor - noopSensors].push_back(t);
		}
	}
	pop.erase(pop.begin() + pop.size() - 1);
	prod.erase(prod.begin() + prod.size() - 1);
  //	while(pop.size() > prod.size()) pop.erase(pop.end() - 1);
  //	while(prod.size() > pop.size()) prod.erase(prod.end() - 1);
  for(int i = 0; i < pop.size(); ++i)
  {
	  dprint("pop  %f prod %f\n", pop[i][0][0], prod[i][0][0]);
	  Sleep(10);
  }
}
#else
void BigArray::Prepare(int popSensors
, unsigned long *inp
, int sizeInp
, vector<vector<int> >(&offset)
, vector<vector<vector<double> > >(&pop)
, vector<vector<vector<double> > >(&prod)
)
{
	int offsPop = 0;//инициализация пореречного результата
	pop.clear();
	vector<vector<double> >pp(popSensors);
	pop.push_back(pp);
	vector<int> &popOffset = offset[0];
	int offsStartPop = popOffset[0];
	int offsPopMax = popOffset.size() - 1;

	int offsProd = 0;//продольный
	prod.clear();
	vector<vector<double> >pr(prodSensors);
	prod.push_back(pr);
	vector<int> &prodOffset = offset[1];
	int offsStartProd = prodOffset[0];
	int offsProdMax = prodOffset.size() - 1;

	int __prodSensors = noopSensors + prodSensors;

	for (int i = 0; i < sizeInp; ++i)
	{
		int sensor = i % countSensors;
		int offs   = i / countSensors;
		if (sensor < popSensors)
		{
			if (offsPop < offsPopMax)
			{
				 if(i >= popOffset[offsPop + 1] && 0 == sensor )
				 {
					  ++offsPop;
					  vector<vector<double > >p(popSensors);
					  pop.push_back(p);
				 }
				 if(i >= offsStartPop)
				 {
					double t = 100.0 * (short)inp[i]/ Globals::level100;
					pop[offsPop][sensor].push_back(t);
				 }
			}
		}
		else if (sensor < noopSensors)
		{
		}
		else if (sensor < __prodSensors)
		{
			if (offsProd < offsProdMax)
			{
				if (i >= prodOffset[offsProd + 1] && noopSensors == sensor)
				{
					++offsProd;
					vector < vector<double> >p(prodSensors);
					prod.push_back(p);
				}
				if(i >= offsStartProd)
				{
					double t = 100.0 * (short)inp[i]/ Globals::level100;
					 int k = sensor - noopSensors;
					 prod[offsProd][k].push_back(t);
				}
			}
        }
	}
	pop.erase(pop.begin() + pop.size() - 1);
	prod.erase(prod.begin() + prod.size() - 1);
	if(pop.size() > prod.size()) pop.erase(pop.end() - 1);
	else if(prod.size() > pop.size()) prod.erase(prod.end() - 1);
}
#endif
#if 0
void BigArray::Prepare(int popSensors
, unsigned long *inp
, int sizeInp
, vector<int>(&popOffset)
, vector<vector<vector<double> > >(&pop)
)
{
	int offsPop = 0;//инициализация пореречного результата
	pop.clear();
	vector<vector<double> >pp(popSensors);
	pop.push_back(pp);
	int offsInpPop = popOffset[0] * popSensors;
	int offsPopMax = popOffset.size() - 1;

	int __prodSensors = noopSensors + prodSensors;

	for (int i = 0; i < sizeInp; ++i)
	{
		int sensor = i % countSensors;
		if (sensor < popSensors)
		{
			if (offsPop < offsPopMax && offsInpPop / popSensors >= popOffset[offsPop + 1])
			{
				++offsPop;
				offsInpPop = popOffset[offsPop] * popSensors;
				vector<vector<double > >p(popSensors);
				pop.push_back(p);
			}
			++offsInpPop;
			double t = 100.0 * (short)inp[i]/ Globals::level100;
			pop[offsPop][sensor].push_back(t);
		}
	}
	pop.erase(pop.begin() + pop.size() - 1);
}
#else
void BigArray::Prepare(int popSensors
, unsigned long *inp
, int sizeInp
, vector<int>(&popOffset)
, vector<vector<vector<double> > >(&pop)
)
{
	int offsPop = 0;//инициализация пореречного результата
	pop.clear();
	vector<vector<double> >pp(popSensors);
	pop.push_back(pp);
	int offsStartPop = popOffset[0];
	int offsPopMax = popOffset.size() - 1;

	for (int i = 0; i < sizeInp; ++i)
	{
		int sensor = i % countSensors;
		int offs   = i / countSensors;
		if (sensor < popSensors)
		{
			if (offsPop < offsPopMax)
			{
				 if(i >= popOffset[offsPop + 1] && 0 == sensor )
				 {
					  ++offsPop;
					  vector<vector<double > >p(popSensors);
					  pop.push_back(p);
				 }
				 if(i >= offsStartPop)
				 {
					double t = 100.0 * (short)inp[i]/ Globals::level100;
					pop[offsPop][sensor].push_back(t);
				 }
			}
		}
	}
	pop.erase(pop.begin() + pop.size() - 1);
}
#endif

void BigArray::TimeToArrayOffsets(
  unsigned *adcTime, int adcCount, int adcFrame
  , unsigned *lirTime, int lirCount
  , vector<int>(&offset)
)
{
	offset.clear();
	int adcOffs = 0;
	for(int i = 0; i < lirCount; ++i)
	{
		while(adcTime[adcOffs] < lirTime[i]) ++adcOffs;
		int pred = adcOffs - 1;
		int a = lirTime[i] - adcTime[pred];
		int b = adcTime[adcOffs] - adcTime[pred];
		double c = (double)a / b;
		c += pred;
		c *= adcFrame;
		//c /= countSensors;

		offset.push_back((int)c);
	 }
}
