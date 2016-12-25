#include "stdafx.h"
#include "In.h"
#include "LexAnalize.h"
#include "Log.h"
#include "Error.h"
#include "Parm.h"
#include "IT.h"
#include "LT.h"
#include "FST.h"
#include "Generation.h"
#include "MFST.h"
#include "GRB.h"
#include "PN.h"

int _tmain(int argc, wchar_t *argv[])
{
	setlocale(LC_CTYPE, "Russian");
	Log::LOG log;
	try
	{
		
		Parm::PARM parm = Parm::getparm(argc, argv); // ���������
		log = Log::getstream(parm.log); // �������� ������
		Log::writeLog(log); // ���������� �������   ���� �����
		Log::writeParm(log, parm); // ������ � ������ ������������ ���������� 
		In::IN in = In::getin(log.stream, parm.in);  // ������ �������� ������� ��������
		in.tokens = In::parsOfToken(in.tokens, in.text, in.code);   // ������������ �� ���������� ����� ���� ��� ���� �������
		Lexer::LEX tables = Lexer::Fill_LTIT(in, log, parm);  // ������ ������������ �������
		MFST::Mfst mfst(tables, GRB::getGreibach()); //������������� ��� ��������������� �����������
		
		if (Log::Error) // ���� ��� ������
		{
			Log::writeIn(log, in); //  ������  ��������� � ���������� �� �����
			Log::writeLexTable(log, tables.Lextable);  // ������ ������� ������
			Log::writeIDtable(log, tables.IDtable); // ������ ������� ���������������
			if (parm.ITtable == true)
			{
				writeIDTableConsole(log, tables.IDtable); // ������ ������� ��������������� � �������
			}
			if (parm.LTtable == true)
			{
				Log::writeLexTableConsole(log, tables.Lextable);  // ������ ������� ������ � �������
			}
			Log::writeIntermediateCode(log, tables.Lextable); // ������ �������������� ����
		}
		if (Log::Error)
		{
			MFST_TRACE_START(log); // �����
			mfst.start(log); //����� ��������������� ������
			mfst.savededucation(); //��������� ������� ������
			mfst.printrules(log); // ������ ������ �������
		}

		if (Log::flaw == false)
		{
			Generation(tables.Lextable, in.tokens);// ������ ��������� � ������������ ����� � ����� JS
			PN polishNatation;
			if (polishNatation.CreatePolishNotation(&tables))
			{
				*log.stream << endl << endl << "Polish Notation";
				Log::writeIntermediateCode(log, tables.Lextable);
			}
		}


		Log::Close(log); // �������� ������
	}

	catch (Error::ERROR e)
	{
		Log::writeError(log.stream, e); // ������

	}
}