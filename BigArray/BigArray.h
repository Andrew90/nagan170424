#pragma once
#include <vector>

using namespace std;
struct BigArray
{
static const int countSensors = 18;//���������� + ���������� + ������ ���������
static const int noopSensors = 12;//����������
static const int prodSensors = 4; //����������
static const int solidSensors = 2; //������ ���������
static void Prepare(int popSensors    //���������� ��������
  , unsigned long *inp    //������ � ����� 791
  , int sizeInp
  , vector<vector<int> >(&offsets) //�������� ���
  , vector<vector<vector<double> > >(&pop)//�������� ������ �� �����������
  , vector<vector<vector<double> > >(&prod) //�������� ������ �� �����������
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

