//---------------------------------------------------------------------------
#pragma hdrstop
#include "Result.h"
#include "IniFiles.hpp"
#include "Global.h"
#include "DebugMess.h"
#include "MedianFiltre.h"
#include "BigArray.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
Result::Result()
{
	int i,j;

   //	DeadZone=0;

	sensors=0;
	zones=0;
	meas_per_zone=0;
	borders.set_length(2);
	dead_zone_start=0;
	dead_zone_finish=0;
	meas_dead_zone_start=0;
	meas_dead_zone_finish=0;
	total_meas = 0;
	zone_data.set_length(Globals::max_zones);
	sensor_data.set_length(Globals::max_zones);
	for (i = 0; i < sensor_data.Length; i++)
		sensor_data[i].set_length(Globals::max_sensors);
	//todo 1605 data.set_length(Globals::max_zones);
	data.resize(Globals::max_zones);
	//todo 1605 for (i = 0; i < data.Length; i++)
	for (i = 0; i < data.size(); i++)
		//todo 1605 data[i].set_length(Globals::max_sensors);
	data[i].resize(Globals::max_sensors);
	//todo 1605 for (i = 0; i < data.Length; i++)
	for (i = 0; i < data.size(); i++)
	   //toto 1605	for (j = 0; j < data[i].Length; j++)
	for (j = 0; j < data[i].size(); j++)
			//todo 1605 data[i][j].set_length(Globals::max_meas);
	data[i][j].resize(Globals::max_meas);
// ������ �����, ������� ����� ����
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	Brack = (TColor) ini->ReadInteger("Color","Brack",0);
	Class2 = (TColor) ini->ReadInteger("Color","SecondClass",0);
	Good = (TColor) ini->ReadInteger("Color","Valid",0);
	Dead = (TColor) ini->ReadInteger("Color","DeadZone",0);
	gain.resize(Globals::max_sensors);
	WidthMedianFilter = ini->ReadInteger("OtherSettings","WidthMedianFilter",5);
	delete ini;

}
//---------------------------------------------------------------------------
Result::~Result()
{
/* todo 1605
	int i,j;
	borders.~DynamicArray();
	zone_data.~DynamicArray();
	for (i = 0; i < sensor_data.Length; i++)
		sensor_data[i].~DynamicArray();
	sensor_data.~DynamicArray();
	for (i = 0; i < data.Length; i++)
		for (j = 0; j < data[i].Length; j++)
			data[i][j].~DynamicArray();
	for (i = 0; i < data.Length; i++)
		data[i].~DynamicArray();
	data.~DynamicArray();
	*/
}
//---------------------------------------------------------------------------
void Result::CreateTestData(short tag)
{
	if (tag==1)
	{
		Result::sensors=12;
		Result::zones=45;
		meas_per_zone=550;
	}
	if (tag==2)
	{
		Result::sensors=Globals::LinSensors;
		Result::zones=50;
		meas_per_zone=1000;
	}
	int i,j,k;
	for (j = 0; j < zones; j++)
	{
		for (i = 0; i < sensors; i++)
		{
			for (k = 0; k < meas_per_zone; k++)
			{
			if ( j%2 == 1 && i%2 == 1)
				data[j][i][k] =  50*Math::Power(System::Sin((double)k/Globals::max_meas*10*2),2);
				//data[j][i][k]=30;
			else
				data[j][i][k]=(double)18.0;
			}
		}
	}
}
//---------------------------------------------------------------------------
#if 0
void Result::ComputeZoneData()
{
	//���� �������� ���������� �� ���������� ���������� ������ � ������ � �� ������ �������� ������ ������
	if(SystemConst::isMedianFilter)
	{
		for(int j=0; j < zones; j++)
		{
			if (SystemConst::IsOnline)
				meas_per_zone = meas_per_sensor[j];
			// ��������� �������, ��� ����� ������ ��� ����������
			double* source_signal=new double[meas_per_zone];
			double* final_signal = new double[meas_per_zone];

				for(int i=0; i < sensors; i++)
				{
					//�������� �� DynamicArray �� ��������� ������ ������, ��� ��������� ����������
					for(int k=0; k < meas_per_zone; k++)
						source_signal[k]=data[j][i][k];
					//����� ����� ��������
					medianfilter(source_signal,final_signal,meas_per_zone);
					//������ ��������������� ������ ������� ������� � ������� ���������� ������
					for(int k=0; k < meas_per_zone; k++)
						data[j][i][k]=final_signal[k];
				}

			delete[] source_signal;
			delete[] final_signal;
		}
	}

// ��������� �� ���� ���������, ��������� ������������� ������� �� ����� � ��������
	double max1 , max2 , max3 = 0;
	for (int j = 0; j < zones; j++)
	{
		max2=0;
		if (SystemConst::IsOnline)
			meas_per_zone = meas_per_sensor[j];
		for (int i = 0; i < sensors; i++)
		{
			max1=0;
			for (int k = 0; k < meas_per_zone; k++)
			{
				if ( data[j][i][k] < 0 )
					data[j][i][k] = -1 * data[j][i][k];
				if ( data[j][i][k] > max1 )
					max1 = data[j][i][k];
			}
			sensor_data[j][i] = max1 * gain[i];
			if ( sensor_data[j][i] > max2 )
				max2 = sensor_data[j][i];
		}
		zone_data[j] = max2 ;
	   //	if (max2 > max3)
	   //		max3 = max2;
	}
	//decision = "";
}
#else
void Result::ComputeZoneData()
{
//���� �������� ���������� �� ���������� ���������� ������ � ������ � �� ������ �������� ������ ������
#if 0
	if(SystemConst::isMedianFilter)
	{
		for(int j=0; j < zones; j++)
		{
			if (SystemConst::IsOnline)
				meas_per_zone = meas_per_sensor[j];
			// ��������� �������, ��� ����� ������ ��� ����������
			double* source_signal=new double[meas_per_zone];
			double* final_signal = new double[meas_per_zone];

				for(int i=0; i < sensors; i++)
				{
					//�������� �� DynamicArray �� ��������� ������ ������, ��� ��������� ����������
					for(int k=0; k < meas_per_zone; k++)
						source_signal[k]=data[j][i][k];
					//����� ����� ��������
					medianfilter(source_signal,final_signal,meas_per_zone);
					//������ ��������������� ������ ������� ������� � ������� ���������� ������
					for(int k=0; k < meas_per_zone; k++)
						data[j][i][k]=final_signal[k];
				}

			delete[] source_signal;
			delete[] final_signal;
		}
	}
#endif
   if(SystemConst::isMedianFilter)
   {
		MedianFiltre m[BigArray::noopSensors];
		for(int i = 0; i < BigArray::noopSensors; ++i)
			m[i].SetWidth(WidthMedianFilter);
		for(int z = 0, len_z = data.size(); z < len_z; ++z)
		{
			 for(int s = 0, len_s = data[z].size(); s < len_s; ++s)
			 {
				for(int i = 0, len_i = data[z][s].size(); i < len_i; ++i)
				{
					  data[z][s][i] = m[s].Do(data[z][s][i]);
				}
			 }
		}
	}

// ��������� �� ���� ���������, ��������� ������������� ������� �� ����� � ��������
#if 0
	double max1 , max2 , max3 = 0;
	for (int j = 0; j < zones; j++)
	{
		max2=0;
		if (SystemConst::IsOnline)
			meas_per_zone = meas_per_sensor[j];
		for (int i = 0; i < sensors; i++)
		{
			max1=0;
			for (int k = 0; k < meas_per_zone; k++)
			{
				if ( data[j][i][k] < 0 )
					data[j][i][k] = -1 * data[j][i][k];
				if ( data[j][i][k] > max1 )
					max1 = data[j][i][k];
			}
			sensor_data[j][i] = max1 * gain[i];
			if ( sensor_data[j][i] > max2 )
				max2 = sensor_data[j][i];
		}
		zone_data[j] = max2 ;
	}
#else
	double max1 , max2 , max3 = 0;
    for(int z = 0, len_z = data.size(); z < len_z; ++z)
	{
	   max2=0;
	   for(int s = 0, len_s = data[z].size(); s < len_s; ++s)
	   {
			max1=0;

			for(int i = 0, len_i = data[z][s].size(); i < len_i; ++i)
			{
				if ( data[z][s][i] < 0 ) data[z][s][i] = -data[z][s][i];
				if ( data[z][s][i] > max1 ) max1 = data[z][s][i];
			}
			sensor_data[z][s] = max1 * gain[s];
			if ( sensor_data[z][s] > max2 ) max2 = sensor_data[z][s];
	   }
	   zone_data[z] = max2 ;
//	   dprint("%s %z %f\n", __FUNCTION__, z, )
	}

	sensors = data[0].size();
	zones = data.size();
#endif
}
#endif
//------------------------------------------------------------------------------
void Result::_medianfilter(double* signal, double* result, int N)
{

   //   Move window through all elements of the signal
   for (int i = 2; i < N - 2; ++i)
   {
	  //   Pick up window elements
	  vector <double> window;
	  window.resize(WidthMedianFilter);
	  for (int j = 0; j < WidthMedianFilter; ++j)
		 window[j] = signal[i - 2 + j];
	  //Order elements (only half of them)
	  for (int j = 0; j < 3; ++j)
	  {
		 //Find position of minimum element
		 int min = j;
		 for (int k = j + 1; k < WidthMedianFilter; ++k)
			if (window[k] < window[min])
			   min = k;
		 //   Put found minimum element in its place
		 const double temp = window[j];
		 window[j] = window[min];
		 window[min] = temp;
	  }
	  //Get result - the middle element
	  result[i - 2] = window[2];
   }
}
//------------------------------------------------------------------------------
//   1D MEDIAN FILTER wrapper
//     signal - input signal
//     result - output signal
//     N      - length of the signal
void Result::medianfilter(double* signal, double* result, int N)
{
   //   Check arguments
   if (!signal || N < 1)
	  return;
   //   Treat special case N = 1
   if (N == 1)
   {
      if (result)
         result[0] = signal[0];
      return;
   }
   //   Allocate memory for signal extension
   double* extension = new double[N + 4];
   //   Check memory allocation
   if (!extension)
      return;
   //   Create signal extension
   memcpy(extension + 2, signal, N * sizeof(double));
   for (int i = 0; i < 2; ++i)
   {
      extension[i] = signal[1 - i];
      extension[N + 2 + i] = signal[N - 1 - i];
   }
   //   Call median filter implementation
   _medianfilter(extension, result ? result : signal, N + 4);
   //   Free memory
   delete[] extension;
}

//----------------------------------------------------------------------------
void Result::AddZone( DynamicArray < DynamicArray <double> > new_data )
{

	double max1=0 , max2=0;
	sensors = (short) new_data.Length;

	if(SystemConst::isMedianFilter)
	{
		if (SystemConst::IsOnline)
			meas_per_zone = new_data[2].Length;
		for(int i=0; i < sensors; i++)
		{
			//����� �������
			int length=new_data[i].Length;
			double* source_signal=new double[length];
			double* final_signal = new double[length];

			//�������� �� DynamicArray �� ��������� ������ ������, ��� ��������� ����������
			for(int k=0; k < length; k++)
				source_signal[k]=new_data[i][k];
			//����� ����� ��������
			medianfilter(source_signal , final_signal , meas_per_zone);
			//������ ��������������� ������ ������� ������� � ������� ���������� ������
			for(int k=0; k < length; k++)
				new_data[i][k]=final_signal[k];

			delete[] source_signal;
			delete[] final_signal;
		}
	}

	for (int i = 0; i < sensors; i++)
	{
		max1=0;
		for (int k = 0; k < new_data[i].Length; k++)
		{
			data[zones][i][k] = new_data[i][k] * 100 / Globals::level100;
			if ( data[zones][i][k] < 0 )
				data[zones][i][k] = -1 * data[zones][i][k];

			if ( data[zones][i][k] > max1 )
				max1 = data[zones][i][k];
		}
		sensor_data[ zones ][ i ] = max1 * gain[i];
		if ( sensor_data[ zones ][ i ] > max2 )
			max2 = sensor_data[ zones ][ i ];
	}

	meas_per_sensor.Length++;
	meas_per_sensor[meas_per_sensor.High] = new_data[2].Length;
	zone_data[ zones ] = max2;
	total_meas += meas_per_sensor[meas_per_sensor.High];
	zones++;
}
//---------------------------------------------------------------------------
#if 0
void Result::CalculateDeadFront(int LengthZone)				// ������� ������� ���� � ������
{
	if (SystemConst::IsOnline)    	// ��� ������ ������������� (���)
	{
		int full_start = Math::Floor((float)dead_zone_start / (float)LengthZone);
		if (zones > 0)
		{
			// ������ ����
			for (int j = 0; j < full_start; j++)
			{
				for (int i = 0; i < sensors; i++)
					sensor_data[j][i] = 1;
				zone_data[j] = 1;
			}
			// �������� ����  �����
			int dead_part = (int) ( (float)(dead_zone_start % LengthZone) / (float)LengthZone * meas_per_sensor[full_start] );
			double max2 = 0;
			for (int i = 0; i < sensors; i++)
			{
				double max1 = 0;
				for (int k = dead_part; k < meas_per_sensor[full_start]; k++)
				{
					if ( data[ full_start ][i][k] > max1 )
						max1 = data[ full_start ][i][k];
				}
				sensor_data[ full_start ][i] = max1 * gain[i];
				if ( sensor_data[ full_start ][i] > max2 )
					max2 = sensor_data[ full_start ][i];
			}
			zone_data [ full_start ] = max2;
			// ��� ������������ �����
			meas_dead_zone_start = 0;
			for (int p = 0; p < full_start; p++)
				meas_dead_zone_start += meas_per_sensor[p];
			meas_dead_zone_start += dead_part;
		}
	}
	else		// ������� (��� �����)
	{
		meas_per_zone = 50;
		meas_dead_zone_start = ( (float) meas_per_zone / (float) LengthZone) * (float) dead_zone_start;
		if ( meas_per_zone )
		{
			// ���-�� ������ ������� ��� � ������
			int dead_zones_start = 	Math::Floor( (float) meas_dead_zone_start / (float) meas_per_zone );
			// ������� ������� ��������� � �������� ����
			int dead_part = (int) meas_dead_zone_start - dead_zones_start * meas_per_zone ;
			for (int j = 0; j < dead_zones_start; j++)
			{
				for (int i = 0; i < sensors; i++)
					sensor_data[j][i] = 1;
				zone_data[j] = 1;
			}

			double max2 = 0;
			for (int i = 0; i < sensors; i++)
			{
				double max1 = 0;
				for (int k = dead_part; k < meas_per_zone; k++)
				{
					if ( data[ dead_zones_start ][i][k] > max1 )
						max1 = data[ dead_zones_start ][i][k];
				}
				sensor_data[ dead_zones_start ][i] = max1 * gain[i];
				if ( sensor_data[ dead_zones_start ][i] > max2 )
					max2 = sensor_data[ dead_zones_start ][i];
			}
			zone_data [ dead_zones_start ] = max2;
		}
	}
}
#else
void Result::CalculateDeadFront(int LengthZone)
{
	int deadZones = LengthZone / Lir::zoneLength;
	int remZone = LengthZone % Lir::zoneLength;

	vector<vector<double> >(&vec) = data[deadZones];

	int sen_len = vec.size();
	for(int z = 0; z < deadZones; ++z)
	{
	   for(int s = 0; s < sen_len; ++s)
	   {
		  sensor_data[z][s] = 1;
	   }
	   zone_data[z] = 1;
	}

	if(remZone)
	{
	double res[20];

	for(int s = 0; s < sen_len; ++s)
	{
		int samplesInZone = vec[s].size();
		int offs = int((double)remZone * samplesInZone / Lir::zoneLength);
		res[s] = vec[s][offs];
		for(int i = offs; i < samplesInZone; ++i)
		{
		   if(res[s] < vec[s][i]) res[s] = vec[s][i];
		}
	}
	double zoneRes = res[0];
	for(int s = 1; s < sen_len; ++s)  if(zoneRes < res[s]) zoneRes = res[s];
	for(int s = 0; s < sen_len; ++s)
	{
		sensor_data[deadZones][s] = res[s];
	}
	zone_data[deadZones] = zoneRes;
	}
};
#endif
//---------------------------------------------------------------------------
#if 0
void Result::CalculateDeadBack(int LengthZone)				// ������� ������� ���� � �����
{
	if (SystemConst::IsOnline)
	{
		int full_finish = 0;
		if (zones > 0)
		{
			double dead_zones = (double)dead_zone_finish / (double)LengthZone;
			// ���� ������ ����� � ��������� �������� ����
			int i;
			for ( i = meas_per_sensor[zones - 1] - 1; i > 0 ; i--)
				if (data[zones-1][0][i] != 0.0)
					break;
			int meas_real = i;		// �������� ��������� � ��������� ����
			double last_part = (double) meas_real / (double) meas_per_sensor[zones - 1];
			double dead_full = dead_zones - last_part;
			while (dead_full > 0.0)		// ����, ������� ������ ��� ������� �������
			{
				dead_full -= 1.0;
				full_finish ++;
			}
			double not_full = 1.0 + dead_full;		// �������� ���� (�� ����������� ���������, ����� ���� = zones-2,-3 � �.�.)
			// ������ ����
			for (int j = 0; j < full_finish; j++)
			{
				for (int i = 0; i < sensors; i++)
					sensor_data[ zones - j -1][i] = 1;
				zone_data[ zones - j -1] = 1;
			}
			// �������� ���� �����
			int dead_part_end = (int) (not_full * (double) meas_per_sensor[zones-full_finish-1]);
			double max2 = 0;
			for (int i = 0; i < sensors; i++)
			{
				double max1 = 0;
				for (int k = 0; k < meas_per_sensor[zones-full_finish-1] - dead_part_end; k++)
				{
					if ( data[ zones - full_finish-1 ][i][k] > max1 )
						max1 = data[ zones - full_finish-1 ][i][k];
				}
				sensor_data[ zones - full_finish-1 ][i] = max1 * gain[i];
				if ( sensor_data[ zones - full_finish-1 ][i] > max2 )
					max2 = sensor_data[ zones - full_finish-1 ][i];
			}
			zone_data [ zones - full_finish-1 ] = max2;

			// ��� ������������ �����
			meas_dead_zone_finish = 0;
			for (int p = 0; p < full_finish; p++)
				meas_dead_zone_finish += meas_per_sensor[zones - p -1];
			meas_dead_zone_finish += dead_part_end;
		}
	}
	else		// ������� (��� ����)
	{
		meas_dead_zone_finish = ( (float) meas_per_zone / (float) LengthZone) * (float) dead_zone_finish;

		//------------- ��������� ������� ����----------
		if ( meas_per_zone )
		{

			// ���-�� ������ ������� ��� � ����� (�����������, ����� ������ ��������� �������� ����)
			int dead_zones_finish = Math::Floor( (float) meas_dead_zone_finish / (float) meas_per_zone );
			// ������� ������� ��������� � �������� ����
			int dead_part_end = (int) meas_dead_zone_finish - dead_zones_finish * meas_per_zone ;
			for (int j = 0; j < dead_zones_finish; j++)
			{
				for (int i = 0; i < sensors; i++)
					sensor_data[ zones - j -1][i] = 1;
				zone_data[ zones - j -1] = 1;
			}
			double max2 = 0;
			for (int i = 0; i < sensors; i++)
			{
				double max1 = 0;
				for (int k = 0; k < meas_per_zone - dead_part_end; k++)
				{
					if ( data[ zones - dead_zones_finish - 1 ][i][k] > max1 )
						max1 = data[ zones - dead_zones_finish - 1 ][i][k];
				}
				sensor_data[ zones - dead_zones_finish - 1 ][i] = max1 * gain[i];
				if ( sensor_data[ zones - dead_zones_finish - 1 ][i] > max2 )
					max2 = sensor_data[ zones - dead_zones_finish - 1 ][i];
			}
			zone_data [ zones - dead_zones_finish - 1 ] = max2;
		}
	}
}
#endif
void Result::CalculateDeadBack(int LengthZone)
{
    int deadZones = LengthZone / Lir::zoneLength;
	int remZone = LengthZone % Lir::zoneLength;


	int zonesCount = data.size();
	int sen_len = data[0].size();
	int offs = zonesCount - deadZones;
	for(int z = offs; z < zonesCount; ++z)
	{
	   for(int s = 0; s < sen_len; ++s)
	   {
		  sensor_data[z][s] = 1;
	   }
	   zone_data[z] = 1;
	}
	--offs;
	vector<vector<double> >(&vec) = data[offs];

	double res[20];
	if(remZone)
	for(int s = 0; s < sen_len; ++s)
	{
		int countSamples = vec[s].size();
		int offsSamples = int((double)remZone * countSamples / Lir::zoneLength);
		res[s] = vec[s][0];
		for(int i = 1; i < countSamples - offsSamples; ++i)
		{
		   if(res[s] < vec[s][i]) res[s] = vec[s][i];
		}
	}
	double zoneRes = res[0];
	for(int s = 1; s < sen_len; ++s)  if(zoneRes < res[s]) zoneRes = res[s];
	for(int s = 0; s < sen_len; ++s)
	{
		sensor_data[offs][s] = res[s];
	}
	zone_data[offs] = zoneRes;
}
//---------------------------------------------------------------------------
void Result::CalculateDeadZone(int LengthZone)
{
	CalculateDeadFront(LengthZone);
	CalculateDeadBack(LengthZone);
}
//---------------------------------------------------------------------------
TColor Result::ZoneColor(double meas,int XCoord)
{
	// ���������� ���� � ����������� �� ���������
	if( ( (XCoord < meas_dead_zone_start) ||
		  (XCoord > (total_meas - meas_dead_zone_finish)) ) &&
		  (XCoord > 0) )
		return Dead;
	else
	if (meas < borders[borders.get_high()])
		return Good;
	else
	if (meas >= borders[0])
		return Brack;
	else
		return Class2;
}
void Result::Clear(TChart *C)
{
    for (int i = 0; i < sensors; i++)
	{
		C->Series[i]->Clear();
    }
}
//---------------------------------------------------------------------------
void Result::PutResultOnChart(TChart *C, TComponent *Owner)
{
// ������ ��������� �� ����� �� ����� �;
// ����� ������� ���� �����������, ��� ���������� ���� ���������� ������ ������ (���� Teechart)
	C->LeftAxis->Maximum = sensors;
	for (int i = 0; i < sensors; i++)
	{
		C->Series[i]->Tag=i;
		((TBarSeries*) C->Series[i])->OnDblClick=SeriesDblClick;
		C->Series[i]->Clear();
		for (int j = 0; j < Globals::max_zones; j++)
			if ( j<zones )
			{
				C->Series[i]->AddXY(j,1,"",ZoneColor( sensor_data[j][i] ));
			 //	dprint("%s %d %d %f\n", __FUNCTION__, j, i, sensor_data[j][i]);
			}
			else
				C->Series[i]->Add(0,"",clWhite);
	}

// �������� ����� ��� ������ ��������� �� ������ �������
	if (!SensorForm)
	{
		SensorForm = new TForm(Owner);
		SensorForm->BorderIcons = TBorderIcons(1<<biSystemMenu);
		SensorForm->OnKeyPress = (TKeyPressEvent) &EscapeKeyPress;
		SensorForm->SetBounds(600,500,300,300);

		SensorChart = new TChart(SensorForm);
		SensorChart->Parent = SensorForm;
		SensorChart->SetBounds( 10, 10 , 260 , 240 );
		SensorChart->Legend->Visible = false;
		SensorChart->View3D = false;
		SensorChart->BottomAxis->Visible = true;
		SensorChart->LeftAxis->Automatic = false;
		SensorChart->LeftAxis->Minimum = 0;
		SensorChart->LeftAxis->Maximum = 100;
		SensorChart->Walls->Back->Transparent = false;
		SensorChart->Walls->Back->Transparency = 30;
		SensorChart->BackWall->Color = clDkGray;
		SensorChart->Align = alClient;
		SensorChart->AddSeries(new TBarSeries(Owner));
		SensorChart->AddSeries(new TLineSeries(Owner));
		SensorChart->AddSeries(new TLineSeries(Owner));
		SensorChart->Series[0]->Marks->Visible = false;
		((TBarSeries*) SensorChart->Series[0])->BarPen->Visible = false;
	}
}
//---------------------------------------------------------------------------
void Result::PutDataOnChart(TChart *Chart,TComponent *Owner, int zone, int sensor)
{
// ������ ��������� � ����/������� �� �������
	Chart->Series[0]->Clear();
	if (SystemConst::IsOnline)
	{
		long temp_meas = 0;
		for(int k = 0; k < zone; k++)
			temp_meas += meas_per_sensor[k];

		for (int j = 0; j < meas_per_sensor[zone]; j++)
		{
			Chart->Series[0]->AddXY( j , data[zone][sensor][j] * gain[sensor],"" ,
				ZoneColor( data[zone][sensor][j] * gain[sensor], temp_meas + j ));
		}
	}
	else
	{
	   //	for (int j = 0; j < meas_per_zone; j++)
	   for (int j = 0, len = data[zone][sensor].size(); j < len; j++)
		{
			Chart->Series[0]->AddXY( j , data[zone][sensor][j] * gain[sensor],"" ,
			//	ZoneColor( data[zone][sensor][j] * gain[sensor], zone*meas_per_zone+j ));
			ZoneColor( data[zone][sensor][j] * gain[sensor], zone*len+j ));
		}
	}
	//SensorChart->Series[0]->AddArray( &data[ValueIndex][sn][0],meas_per_zone-1);
// ������ ������ ����� � ������ 2, ���� �� �������
	Chart->Series[1]->Clear();
	Chart->Series[2]->Clear();
	for (int j = 0; j < borders.Length; j++)
	{
		Chart->Series[j+1]-> AddXY( 0 , borders[j] ,"" , ZoneColor( borders[j] ) );
		Chart->Series[j+1]-> AddXY(Chart->Series[0]->MaxXValue() , borders[j] , "" , ZoneColor( borders[j] ) );
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall Result::SeriesDblClick(TChartSeries *Sender, int ValueIndex, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
// ����������� ���������� ������������� ������
	Sender->ValueColor[ValueIndex]=clNavy;
// � ���� �������� ����� ������� (����� �����)
	int sn = Sender->Tag;
	SensorForm->Left = 600;		// Sender->ParentChart->Parent->Left+X+30;
	SensorForm->Top = 500;			// (Sender->ParentChart->Parent->Top) + (Sender->ParentChart->Height+100)
	SensorForm->Width = 300;
	SensorForm->Height = 300;
	SensorForm->Caption="������ "+IntToStr(sn+1)+", ���� "+IntToStr(ValueIndex+1);//+" x="+IntToStr(X)+" y="+IntToStr(Y);
//	�������� ��������� �� ������
	PutDataOnChart(SensorChart, Sender, ValueIndex, sn);
	SensorForm->Update();
	SensorForm->Refresh();
	SensorForm->Repaint();
	SensorForm->Show();
}
//----------------------------------------------------------------------------
void __fastcall Result::EscapeKeyPress(TObject *Sender, wchar_t &Key)
{
	if (Key == 27) ((TForm*)Sender)->Close();
}
//------------------------------------------------------------------------------
void Result::SaveTubeToFile(UnicodeString FileName)
{
	//������ ����� .dkb ��� ������������
	//1/0 ����� RunTime/Normal
	//zones,sensors
	//meas_per_zone / meas_per_sensor[]
	//zone_data[]
	//sensor_data[][]
	//data[][][]

	FILE *file;
	file=fopen(AnsiString(FileName).c_str(), "a");
	if( file == NULL )
		Application->MessageBoxW( L"�� ������� ������� ���� ��� ������",L"������", MB_ICONERROR | MB_OK  );

	if (SystemConst::IsOnline)
		fprintf(file,"%d",1);                                       //������ �����, ���� ��� RunTime �����
	else
		fprintf(file,"%d",0);                                       //��� ������� �����������
	fprintf(file,"\n");
	fprintf(file,"%d %d ", zones, sensors);                         //���-�� ���,��������

	if (SystemConst::IsOnline)                                      //���� ����������� RunTime, �� ����� ����� ������ �� ���
		for (int i = 0; i < zones; i++)
			fprintf( file,"%d ",meas_per_sensor[i] );
	else
		fprintf(file, "%d",meas_per_zone);							//����� ������ ���������� ����� ����� ����

	//����� ������ �� �����
	fprintf(file,"\n");
	for( int i=0; i< zones; i++ )
		fprintf(file,"%4.1f ",zone_data[i]);
	fprintf(file,"\n");
	//����� ������ �� ��������
	for ( int i = 0; i <zones ; i++)
		for(int j=0; j<sensors; j++)
			fprintf(file,"%4.1f ",sensor_data[i][j]);

	//����� ��� ��������� ������
	fprintf(file,"\n");
	for (int j = 0; j < zones; j++)                                 //����
	{
		fprintf(file,"\n");
		for (int i = 0; i < sensors; i++)                           //������
		{
		if (SystemConst::IsOnline)                                  //���� RunTime, �� � ������ ���� ������ ���-�� ���������
			for (int k = 0; k < meas_per_sensor[j]; k++)
				fprintf(file,"%4.1f ",data[j][i][k]);
		else                                                        //���� �������, �� ����� ���-�� ��������� � ����� ����
			for (int k = 0; k < meas_per_zone; k++)
				fprintf(file,"%4.1f ",data[j][i][k]);
		}
	}
	fclose(file);
}
//------------------------------------------------------------------------------
void Result::LoadTubeFromFile(FILE*file)
{
	int isRunTime=0;

	fscanf(file,"%d",&isRunTime);                       //������� � ����� ������� ��� �������� ���� ( 1-RunTime,0-normal )
	if( isRunTime > 1 )                                 //���� ������ ������ ������ - Activated Spike-nail
		{
			isRunTime = 0;
			rewind(file);
		}

	fscanf(file,"%d%d",&zones,&sensors);				//���-�� ��� � ��������

	if (isRunTime)
	{
		meas_per_sensor.set_length(zones);
		for( int i=0; i < zones; i++ )                  //���� ����������� RunTime, �� ������ ������ ���� ������ ����
		{
			fscanf(file,"%d", &meas_per_sensor[i]);
			total_meas += meas_per_sensor[i];
		}
	}
	else
	{
		fscanf(file,"%d",&meas_per_zone);               //���� ������� �� ������ ������� ����� ����
		total_meas = meas_per_zone * zones;
		// �������� �������� ��� ������
		meas_per_sensor.set_length(zones);
		for (int i = 0; i < zones; i++)
			meas_per_sensor[i] = meas_per_zone;
	}
	//������ ������ �� �����
	for( int i=0; i<zones; i++ )
		fscanf(file,"%lf ",&zone_data[i]);

	//������ ������ �� ��������
	for ( int i = 0; i <zones ; i++)
		for(int j=0; j<sensors; j++)
			fscanf(file,"%lf",&sensor_data[i][j]);

	//������ ��� ��������� ������
	for (int j = 0; j < zones; j++)
	{
		for (int i = 0; i < sensors; i++)
			if (isRunTime)
				for (int k = 0; k < meas_per_sensor[j]; k++)
					fscanf(file,"%lf ",&data[j][i][k]);
			else
				for (int k = 0; k < meas_per_zone; k++)
					fscanf(file,"%lf ",&data[j][i][k]);
	}

//���� �� ���������, �� ����� ������ � ������� ������� ���������� ��������� �� �������� ����
}
//----------------------------------------------------------------------------
void Result::DeleteData()
{
/*
	int i,j,k;
	for (i = 0; i < data.Length; i++)
		for (j = 0; j < data[i].Length; j++)
			for (k = 0; k < data[i][j].Length; k++)
				data[i][j][k] = 0;

	for (i = 0; i < sensor_data.Length; i++)
		for (j = 0; j < sensor_data[i].Length; j++)
			sensor_data[i][j] = 0;

	for (i = 0; i < zone_data.Length; i++)
		zone_data[i] = 0;
 */
/*
	zone_data.set_length( 0 );
	sensor_data.set_length( 0 );
	data.set_length( 0 );

	zone_data.set_length(Globals::max_zones);
	sensor_data.set_length(Globals::max_zones);
	data.set_length(Globals::max_zones);

	int q = zone_data.Length;
	int w = sensor_data.Length;
	int e = data.Length;
*/
	meas_per_sensor.set_length( 0 );
	zone_data.set_length( 0 );
    zone_data.set_length(Globals::max_zones);
	zones = 0;
	sensors = 0;
	meas_per_zone = 0;
	total_meas = 0;
}
//----------------------------------------------------------------------------

void Result::SetGains(vector <double> _Gain)
{
	gain = _Gain;
}

//----------------------------------------------------------------------------
void Result::SetSensorGain(double _gain, int sensor)
{
	gain [sensor] = _gain;
}

//----------------------------------------------------------------------------
//-------------������ ����������  - ThicknessResult--------------------------
//----------------------------------------------------------------------------
ThicknessResult::ThicknessResult()
{
	ThicknessResult::zones=0;
	ThicknessResult::zone_data.set_length(Globals::max_zones);
}
//----------------------------------------------------------------------------
ThicknessResult::~ThicknessResult()
{
	ThicknessResult::zone_data.~DynamicArray();
}
//----------------------------------------------------------------------------
void ThicknessResult::CreateTestData(short tag)
{
	ThicknessResult::sensors=6;
	ThicknessResult::zones=49;
	int i,j,k;
	for (j = 0; j < ThicknessResult::zones; j++) {
		for (i = 0; i < ThicknessResult::sensors; i++) {
			for (k = 0; k < Globals::max_meas; k++) {
				if ( j ==4)
					data[j][i][k]=4;
				else if (j==35)
					data[j][i][k]=4;
				else if (j==25)
					data[j][i][k]=5;
				else
					data[j][i][k]=6.0;
			}
		}
	}
}
//---------------------------------------------------------------------------
void ThicknessResult::ComputeZoneData()
{
	double min1,min2;
	for (int j = 0; j < zones; j++)
	{
		min2=1000;
		for (int i = 0; i < sensors; i++)
		{
			min1=1000;
			for (int k = 0; k < Globals::max_meas; k++)
				if (data[j][i][k]<min1)
					min1=data[j][i][k];
			if (min1<min2) min2=min1;
		}
		zone_data[j]=min2;
	}
}
//----------------------------------------------------------------------------
TColor ThicknessResult::ZoneColor(double meas)
{
// ���������� ���� ���� � ����������� �� ������� � ���� ����
	if ( meas == 8.0 )
		return Dead;
	else if (meas >= ThicknessResult::borders[borders.get_high()])
		return Good;
	else if (meas < ThicknessResult::borders[0])
		return Brack;
	else
		return Class2;
}
//----------------------------------------------------------------------------
void ThicknessResult::Clear(TChart *Chart)
{
		Chart->Series[0]->Clear();
}
void ThicknessResult::PutResultOnChart(TChart *Chart, TComponent *Owner)
{
// ������ ��������� �� ����� �� ����� �;
	Chart->Series[0]->Clear();
	for (int j = 0; j <= Globals::max_zones; j++)
		if ( j<zones )
			((TBarSeries*) Chart->Series[0])->AddXY(j,zone_data[j],"",ZoneColor(zone_data[j]));
		else
			Chart->Series[0]->Add(0,"",clWhite);
}
//------------------------------------------------------------------------------
void ThicknessResult::SaveTubeToFile(UnicodeString FileName)
{
	FILE *file;
	file=fopen(AnsiString(FileName).c_str(), "a");
	if( file == NULL )
		Application->MessageBoxW( L"�� ������� ������� ���� ��� ������",L"������", MB_ICONERROR | MB_OK  );
	fprintf(file,"\n");
	fprintf(file,"%d ",zones);
	for(int i=0; i < zones; i++)
		fprintf(file, "%2.2f ", zone_data[i]);
	fclose(file);
}
//------------------------------------------------------------------------------
void ThicknessResult::LoadTubeFromFile(FILE *file)
{
	fscanf(file,"%d ",&zones);
	for(int i=0; i < zones; i++)
	{
		double ppp;
//		fscanf(file,"%lf", &zone_data[i]);
		fscanf(file,"%lf", &ppp);
		zone_data[i] = ppp;
	}
}
//------------------------------------------------------------------------------
//----------------------------------------------------------------------------
//--------------������ ���������� - ������ SummaryResult----------------------
//---------------------------------------------------------------------------
SummaryResult::SummaryResult()
{
	SummaryResult::zones = 0;
	SummaryResult::zone_data.set_length(Globals::max_zones);
	cut1 = cut2 = 0;
	decision = "";
}
//---------------------------------------------------------------------------
SummaryResult::~SummaryResult()
{
	SummaryResult::zone_data.~DynamicArray();
}
//---------------------------------------------------------------------------
void SummaryResult::ComputeZonesData(Result &_pp, Result &_pr, ThicknessResult &_th, bool bPr, bool bThick)
{
// ������� ��������� ������� �� ���� �� ������������ �������� � �������
// �������, ����� ������ ��������� ������� � ������
	bool lin = bPr && _pr.zones > 0;
	bool th = bThick && _th.zones > 0;
	if ( lin && th )
		SummaryResult::zones = (short) Min( _pp.zones, Min( _pr.zones, _th.zones ) );
	else if ( lin && !th )
		SummaryResult::zones = (short) Min( _pp.zones, _pr.zones );
	else if ( !lin && th )
		SummaryResult::zones = (short) Min( _pp.zones, _th.zones );
	else if ( !lin && !th )
		SummaryResult::zones = _pp.zones;

// 1 - �����, 0 - ����, 2 - 2 �����, 3 - 3 �����...
	for (int i = 0; i < SummaryResult::zones; i++)
	{
		if ( _pp.zone_data[i] >= _pp.borders[0] ||
				( (lin) && _pr.zone_data[i] >= _pr.borders[0] ) ||
				( (th) && _th.zone_data[i] < _th.borders[0] ) )
		{
			SummaryResult::zone_data[i]=0;
		}
		else if ( _pp.zone_data[i] < _pp.borders[_pp.borders.get_high()] &&
					( (!lin) || _pr.zone_data[i] < _pr.borders[_pr.borders.get_high()] ) &&
					( (!th) || _th.zone_data[i] >= _th.borders[_th.borders.get_high()]) )
		{
			SummaryResult::zone_data[i]=1;
		}
		else
			SummaryResult::zone_data[i]=2;
	}
}
//---------------------------------------------------------------------------
TColor SummaryResult::ZoneColor(double meas)
{
// ���������� ���� ���� � ����������� �� ���������� ���������� � ���� ����
	if (meas == 1)
		return Good;
	else if (meas == 0)
		return Brack;
	else
		return Class2;
}
//---------------------------------------------------------------------------
void SummaryResult::PutResultOnChart(TChart *Chart, TComponent *Owner)
{
// ������ ��������� �� ����� �� ����� �;
	Chart->Series[0]->Clear();
	for (int j = 0; j <= Globals::max_zones; j++)
		if ( j < zones)
			((TBarSeries*) Chart->Series[0])->Add(1,"",ZoneColor(zone_data[j]));
		else
			Chart->Series[0]->Add(0,"",clWhite);
	Chart->Refresh();
}
//---------------------------------------------------------------------------
void SummaryResult::MakeDecision()
{
// ��������� ������� �� �����, ��������� �������� �����
	bool flag=false;		// �������������, ��� ������� ������� ����� ������� 2-�� �������, ���� �������� ����������� ������ �����
	bool class2=false;		// ������� ����� ����� ������� 2-�� �������
	short temp=0;			// ����� �������� ������� � �����
	short tcut1=0; 			// ��������� ������ ���
	cut1=cut2=0;			// ����� �� ��������� ���
	decision="";			// ������� �� �������
	if (min_good_length > 0)
	{
		for (int i = 0; i < zones; i++)
		{
			if (zone_data[i]==1)
				temp++;
			else if (zone_data[i]==0)
			{
				if (temp>=min_good_length)
				{
					cut2 = (short) i;
					cut1 = (short) i-temp;
					class2=flag;
				}
				temp=0;
				tcut1=i+1;
				flag=false;
			}
			else if (zone_data[i]==2)
			{
				flag=true;
				temp++;
			}
		}

		if ( temp < min_good_length && cut2==0 && cut1==0 )
			decision = "����";
		else if ( temp >= min_good_length )
		{
			if (flag) decision = "����� 2";
			else decision = "�����";
			cut1 = tcut1;
			cut2 = 0;
		}
		else if ( cut1>0 || cut2>0 )
		{
			if (class2) decision = "����� 2";
			else decision = "�����";
		}

		// ������ ����-�� ������
		if (end_cut > 0 && decision != "����")
		{
			if (cut2 == 0)
				cut2 = zones;
			int f1 = ( end_cut - cut1 > 0) ? (end_cut-cut1) : (0);
			int f2 = ( end_cut - zones + cut2 > 0 ) ? (end_cut-zones+cut2) : (0);
			if ( cut2 - cut1 - f1 - f2 < min_good_length )
				decision = "����";
		}
	}
	// ������ �� ������� - ����� ���� - ����� �����
	else
	{
		bool brack = false;
		for (int i = 0; i < zones; i++)
		{
			if (zone_data[i] == 0)
			{
				brack = true;
				break;
			}
			else if (zone_data[i] == 2)
				class2 = true;
		}
		if (brack)
			decision = "����";
		else if (class2)
			decision = "����� 2";
		else
	        decision = "�����";
    }
}
//---------------------------------------------------------------------------
void SummaryResult::AddZone(Result &_pp, Result &_pr, ThicknessResult &_th)
{
// ������� ��������� ������� �� ���� �� ������������ �������� � �������
// �������, ����� ������ ��������� ������� � ������
	bool lin = _pr.zones > 0;
	bool th = _th.zones > 0;

// 1 - �����, 0 - ����, 2 - 2 �����, 3 - 3 �����...
	int i =	zones;
	if ( _pp.zone_data[i] >= _pp.borders[0] ||
			( (lin) && _pr.zone_data[i] >= _pr.borders[0] ) ||
			( (th) && _th.zone_data[i] < _th.borders[0] ) )
	{
		SummaryResult::zone_data[i]=0;
	}
	else if ( _pp.zone_data[i] < _pp.borders[_pp.borders.get_high()] &&
				( (!lin) || _pr.zone_data[i] < _pr.borders[_pr.borders.get_high()] ) &&
				( (!th) || _th.zone_data[i] >= _th.borders[_th.borders.get_high()]) )
	{
		SummaryResult::zone_data[i]=1;
	}
	else
		SummaryResult::zone_data[i]=2;

	zones++;

}
//---------------------------------------------------------------------------

