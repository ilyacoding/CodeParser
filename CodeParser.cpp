// CodeParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "fstream"
#include "string"
#include "vector"
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
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
int OperatorsUsed = 0;

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


void EraseEmptyStrings()
{
	for (int i = 0; i < s.size() - 1; i++)
	{
		string str = s[i];
		boost::algorithm::trim(str);
		if (str.length() < 1)
		{
			s.erase(s.begin() + i);
			i--;
		}
	}
}

void Process(int CurrLine)
{
	int CurrSpaces = (CountLeftSpaces(s[CurrLine]));
	s.insert(s.begin() + CurrLine, AddSpaces("{", CurrSpaces));
	OperatorsUsed++;
	cout << CurrLine << endl;
	while ((CountLeftSpaces(s[++CurrLine]) == CurrSpaces) && (CurrLine < s.size() - 1));
	cout << CurrSpaces << endl << CountLeftSpaces(s[CurrLine]) << endl;

	if (CurrSpaces > CountLeftSpaces(s[CurrLine]))
	{
		cout << CurrSpaces << endl << CountLeftSpaces(s[CurrLine]) << endl;
		s.insert(s.begin() + CurrLine, AddSpaces("}", CurrSpaces));
		OperatorsUsed++;
	}
	else if (CurrSpaces < CountLeftSpaces(s[CurrLine])) {
		Process(CurrLine);
		vector <int> queue;
		int DefSpaces = 1;
		while ((CountLeftSpaces(s[++CurrLine]) >= CurrSpaces) && (CurrLine < s.size() - 1))
			if ((CountLeftSpaces(s[CurrLine]) > CurrSpaces) && (DefSpaces))
			{
				DefSpaces = CountLeftSpaces(s[CurrLine]);
				queue.push_back(CurrLine + OperatorsUsed);
			}
			

		s.insert(s.begin() + CurrLine, AddSpaces("}", CurrSpaces));
		OperatorsUsed++;

		for (int i = 0; i < queue.size() - 1; i++)
		{
			cout << "QUEUE: " << queue[i] << endl;
			//if (queue[i] < s.size() - 1)
				//Process(queue[i]);
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

	EraseEmptyStrings();
	Process(0);
	//Process(14);
/*
	int CurrLine = 4;
	int CurrSpaces = (CountLeftSpaces(s[CurrLine]));
	s.insert(s.begin() + CurrLine, AddSpaces("{", CurrSpaces));
	cout << CurrLine << endl;
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

		*/





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

