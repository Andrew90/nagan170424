//---------------------------------------------------------------------------


#pragma hdrstop
#include "Advantech1784.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall Advantech1784::Advantech1784(bool CreateSuspended)
{
	Error="��� ���";
	ULONG   DevNum = 0;                  	//����� ����������
	ini = new TIniFile(Globals::IniFileName);
	for (int i = 0; i < Globals::Counters; i++)
	{
		Channel[i] = ini->ReadInteger("LIR","Channel_" + IntToStr(i), i );
		Counter[i] = 0;
	}
	delete ini;

	m_bContinue=true;
	// ��������� �����
	m_ErrCde=DRV_DeviceOpen(DevNum, &DriverHandle);
	if ( m_ErrCde != SUCCESS )
	{
		Error="�� ������� ������� ����� Advantech1784";
		ProtocolForm->SendToProtocol(Error);
		DRV_GetErrorMessage(m_ErrCde,Errmsg);
		ProtocolForm->SendToProtocol((String)Errmsg);
		m_bContinue=false;
	}

	// ������������� �� x1 �������, � �� x4 �� ���������
	long prop[4] = {1,1,1,1};
	m_ErrCde = DRV_DeviceSetProperty(DriverHandle, CFG_CntrInputModeControl, prop, sizeof(prop));
	if ( m_ErrCde != SUCCESS )
	{
		ProtocolForm->SendToProtocol("�� ������� ���������� ��������� 1784");
		DRV_GetErrorMessage(m_ErrCde,Errmsg);
		ProtocolForm->SendToProtocol((String)Errmsg);
		m_bContinue=false;
	}

	// new stuff
	PT_CounterEventStart ptCounterEventStart;
	for (int i = 0; i < Globals::Counters; i++)
	{
		ptCounterEventStart.counter = i;
		m_ErrCde = DRV_CounterEventStart(DriverHandle, &ptCounterEventStart);
		if ( m_ErrCde != SUCCESS )
		{
			ProtocolForm->SendToProtocol("�� ������� ���������� ������� " + IntToStr(i));
			DRV_GetErrorMessage(m_ErrCde,Errmsg);
			ProtocolForm->SendToProtocol((String)Errmsg);
			m_bContinue=false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall Advantech1784::Execute()
{
	MyThreadCounter();
}
//---------------------------------------------------------------------------
ULONG Advantech1784::GetCounter(int num)
{
	return Counter[ Channel[num] ];
}
//---------------------------------------------------------------------------

bool Advantech1784::CloseDevice()
{
	if ( DriverHandle != NULL )
	{
		m_ErrCde = DRV_DeviceClose( &DriverHandle );
		if (m_ErrCde == SUCCESS)
			return true;
		else
		{
			DRV_GetErrorMessage(m_ErrCde , Errmsg);
			ProtocolForm->SendToProtocol("�� ������� ������� ����� 1784");
			ProtocolForm->SendToProtocol(Errmsg);
			return false;
		}
	}
	return true;
}
//---------------------------------------------------------------------------
bool Advantech1784::Reset(int NumOfCounter)
{
	bool r = true;
	if (NumOfCounter == -1)		// �� ��������� ��������� ��� ��������
	{
		for (int i = 0; i < Globals::Counters; i++)
		{
			m_ErrCde = DRV_CounterReset(DriverHandle,i);
			if ( m_ErrCde != SUCCESS)
			{
				ProtocolForm->SendToProtocol("�� ������� �������� �������� �������� " + IntToStr(i));
				DRV_GetErrorMessage(m_ErrCde , Errmsg);
				ProtocolForm->SendToProtocol(Errmsg);
				r = false;
			}
		}
	}
	else if (NumOfCounter > (Globals::Counters-1) )		// ��� ������ ��������
		return false;
	else				// �������� ���������� �������
	{
		m_ErrCde = DRV_CounterReset(DriverHandle , Channel [NumOfCounter]);
		if ( m_ErrCde != SUCCESS)
		{
			ProtocolForm->SendToProtocol("�� ������� �������� �������� �������� " + IntToStr(NumOfCounter));
			DRV_GetErrorMessage(m_ErrCde , Errmsg);
			ProtocolForm->SendToProtocol(Errmsg);
			r = false;
		}
	}
	return r;
}
//---------------------------------------------------------------------------
void Advantech1784::MyThreadCounter(void)
{
	// �������������� ����� ���������
	PT_CounterEventRead ptCounterEventRead[Globals::Counters];
	ULONG ulCount[Globals::Counters];
	USHORT usOverflow[Globals::Counters];

	for (int i = 0; i < Globals::Counters; i++)
	{
		ptCounterEventRead[i].counter = i;
		ptCounterEventRead[i].count = &ulCount[i];
		ptCounterEventRead[i].overflow = &usOverflow[i];
	}
	// ����������� ���� �� ������ ���������
	while (m_bContinue)
	{
		for (int i = 0; i < Globals::Counters; i++)
		{
			m_ErrCde = DRV_CounterEventRead(DriverHandle, &ptCounterEventRead[i]);
			if ( m_ErrCde == SUCCESS )
				Counter[i] = ulCount[i];

			else
			{
				ProtocolForm->SendToProtocol("������ � ����� 1784, �������: " + IntToStr(i)+
											 ", � ������: "+IntToStr((int)m_ErrCde));
				DRV_GetErrorMessage(m_ErrCde , Errmsg);
				ProtocolForm->SendToProtocol(Errmsg);
				m_bContinue=false;
			}
		}

		TThread::CurrentThread->Sleep(1);
		if (Terminated)
			m_bContinue = false;
	}
	TThread::CurrentThread->Sleep(1000);
	bool cl = CloseDevice();
	SetReturnValue(cl);
}


