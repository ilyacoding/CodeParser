// CodeParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "fstream"
#include "string"
#include "vector"
#include "boost/algorithm/string.hpp"

using namespace std;

enum Token {
	BEGIN, END, IF
};

struct Lexem {
	Token tok;
	//string value;
	//Lexem() :tok(BEGIN) {};
	Lexem(Token tk) :tok(tk) {};
};

vector <Lexem> v;
vector <string> s;
//int CurrLine = 0;

int CountLeftSpaces(string str) 
{
	int i = 0;
	while (str[i] == ' ')
		i++;
	return i;
}

string AddSpaces(string str, int count)
{
	for (int i = 0; i < count; i++)
		str = ' ' + str;
	return str;
}

void ProcessBlock(int CurrLine, int PredSpaces)
{
	int CurrSpaces = (CountLeftSpaces(s[CurrLine]));
	while (CurrLine < s.size())
	{
		if (CountLeftSpaces(s[CurrLine]) > CurrSpaces) {
			for (int j = 0; j < CurrSpaces; j++)
				cout << " ";
			cout << "BEGIN" << endl;
			cout << s[CurrLine] << endl;
			ProcessBlock(CurrLine, CurrSpaces);
			while (PredSpaces < (CountLeftSpaces(s[CurrLine++])));
			for (int j = 0; j < CurrSpaces; j++)
				cout << " ";
			cout << "END" << endl;
			break;
		}
		else if (CurrSpaces > CountLeftSpaces(s[CurrLine])) {
			for (int j = 0; j < CurrSpaces; j++)
				cout << " ";
			cout << "END" << endl;
			ProcessBlock(CurrLine, CurrSpaces);
			break;
		}
		else { cout << s[CurrLine] << endl; }
		CurrLine++;
	}
}

void Process(int CurrLine, int PredSpaces)
{
	int CurrSpaces = (CountLeftSpaces(s[CurrLine]));
	s[CurrLine] = ";BEGIN;" + s[CurrLine];
	while (CurrLine < s.size() - 1)
	{
		while (CountLeftSpaces(s[CurrLine++]) == CurrSpaces);
		if (CurrSpaces > CountLeftSpaces(s[CurrLine]))
		{
			s[CurrLine] = ";END;" + s[CurrLine];
			break;
		}
		else if (CurrSpaces < CountLeftSpaces(s[CurrLine])) {
			Process(CurrLine, CurrSpaces);
		}
		else {
			CurrLine++;
		}
	
	}
}

int main()
{
	setlocale(LC_ALL, "rus");
	ifstream file;
	char buf[500];
	string str = "";
	file.open("Program.fs", ios_base::in);
	while (!file.eof()) {
		file.getline(buf, 500);
		s.push_back(buf);
	}

	int CurrLine = 7;
	int CurrSpaces = (CountLeftSpaces(s[CurrLine]));
	s.insert(s.begin() + CurrLine, AddSpaces("{", CurrSpaces));
	cout << CurrLine << endl;
	//s[CurrLine] = ";BEGIN;" + s[CurrLine];
	//while (CurrLine < s.size() - 1)
	//{
		while ((CountLeftSpaces(s[++CurrLine]) == CurrSpaces) && (CurrLine < s.size() - 1));
		cout << CurrSpaces << endl << CountLeftSpaces(s[CurrLine]) << endl;
		
		if (CurrSpaces > CountLeftSpaces(s[CurrLine]))
		{		
			cout << CurrSpaces << endl << CountLeftSpaces(s[CurrLine]) << endl;
			s.insert(s.begin() + CurrLine, AddSpaces("}", CurrSpaces));
			//break;
		}
		else if (CurrSpaces < CountLeftSpaces(s[CurrLine])) {
		//	Process(CurrLine, CurrSpaces);
			//s[CurrLine] = "GO HERE" + s[CurrLine];
			while ((CountLeftSpaces(s[++CurrLine]) >= CurrSpaces) && (CurrLine < s.size() - 1));
			s.insert(s.begin() + CurrLine, AddSpaces("}", CurrSpaces));
		}
	//	else {
	//		CurrLine++;
	//	}
	//	break;
	//}

	//ProcessBlock(0, 0);
	/*for (int i = 0; i < s.size(); i++) {
		CurrS = CountLeftSpaces(s[i]);

		if (CurrS > PredS) {
			v.push_back(Lexem(BEGIN));
			for (int j = 0; j < PredS; j++)
				cout << " ";
			cout << "BEGIN" << endl;
		} else if (PredS > CurrS) {
			if (CurrS > 0) {
				for (int k = 1; k < PredS / CurrS; k++) {
					v.push_back(Lexem(END));
					for (int j = 0; j < CurrS; j++)
						cout << " ";
					cout << "END" << endl;
				}
			}
		}*/
	//Process(0, 0);
	for (int i = 0; i < s.size(); i++)
	{
		cout << s[i] << endl;
	}

		//cout << CountSpaces(s[i]);
		//cout << s[i] << endl;
		//PredS = CurrS;
	//}

	file.close();

	system("pause");
    return 0;
}

