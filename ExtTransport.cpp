//---------------------------------------------------------------------------


#pragma hdrstop

#include "ExtTransport.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

ExtTransport::ExtTransport(){}
//---------------------------------------------------------------------------
String ExtTransport::Load()
{
	if( !digital->CheckInSignal("�� �������") )
		return "��� ����� �� �������";
	else if( digital->CheckInSignal("�� ������ �����") )
		return "����� �� �����";
	else
	{
		return WorkLoader(1);
    }
}
//---------------------------------------------------------------------------
String ExtTransport::UnloadTubeOutOfLine()
{
	String Value = NET;
	if (SystemConst::ExternalTransport)
	{
		Value = ok;
		if( digital->CheckInSignal("�� ����� �����") && !digital->CheckInSignal("�� ���������") )
		{
			Value = WorkLoader(2);
			for(int i=0; i < 3; i++)
			{
				if( (digital->CheckInSignal("�� ����� �����") && !digital->CheckInSignal("�� ���������") ) ||
				(!digital->CheckInSignal("�� ����� �����") && !digital->CheckInSignal("�� ���������")) )
					{
						Value = WorkLoader(2);
						TThread::CurrentThread->Sleep(3000);
					}
			}
			if( (digital->CheckInSignal("�� ����� �����") && !digital->CheckInSignal("�� ���������") ) ||
				(!digital->CheckInSignal("�� ����� �����") && !digital->CheckInSignal("�� ���������")) )
			Value = "�� ������� ���������� �����";
			return Value;
		}
	}
	return Value;
}
//---------------------------------------------------------------------------
String ExtTransport::WorkLoader(int number)
{
	String n = IntToStr(number);
	digital->ResetOutSignal("�� ������"+n+" ���");
	if(LoaderTest(number) == ok)
	{
		digital->BlinkOutSignal("�� ������"+n+" ���" , 2000);
		if( !digital->WaitForInSignal("�� ������"+n+" ������", 8000) )
			return "������ ��� �������� �����";

		digital->BlinkOutSignal("�� ������"+n+" ����" , 2000);
		if( !digital->WaitForInSignal("�� ������"+n+" �����", 8000) )
			return "������ ��� �������� ����";
		else
			return ok;
	}
	else
		return ErrLoader;

}
//---------------------------------------------------------------------------
String ExtTransport::LoaderTest(int number)
{
	String n = IntToStr(number);
	if(digital->CheckInSignal("�� ������"+n+" ������") ) 				//���� ������
	{
		 digital->BlinkOutSignal("�� ������"+n+" ����" , 120);                  //������� ����

		 if( !digital->WaitForInSignal("�� ������"+n+" �����", 8000) )
			return ErrLoader+n;
		 else
			return ok;
	}
	else
		return ok;
}
//---------------------------------------------------------------------------
void ExtTransport::ResetAllExtTransportSignal()
{
	digital->ResetOutSignal("�� �������� 1");
	digital->ResetOutSignal("�� �������� 2");
	digital->ResetOutSignal("�� �������� 3");
	digital->ResetOutSignal("�� �������� ������");
	digital->ResetOutSignal("�� ������1 ���");
	digital->ResetOutSignal("�� ������1 ����");
	digital->ResetOutSignal("�� ������2 ���");
	digital->ResetOutSignal("�� ������2 ����");
}
//---------------------------------------------------------------------------
void ExtTransport::StopMovingExtTransport()
{
	digital->ResetOutSignal("�� �������� 1");
	digital->ResetOutSignal("�� �������� 2");
	digital->ResetOutSignal("�� �������� 3");
	digital->ResetOutSignal("�� �������� ������");
    digital->ResetOutSignal("�� �������� �����");
}
//---------------------------------------------------------------------------
