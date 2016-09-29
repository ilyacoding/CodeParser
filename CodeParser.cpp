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

void DeleteComments()
{
	bool Delete = false;
	for (int i = 0; i < s.size() - 1; i++)
	{
		for (int j = 0; j < s[i].length(); j++) 
		{
			if (Delete) {
				if (s[i][j] == '*' && s[i][j + 1] == ')') {
					Delete = false;
					s[i][j++] = ' ';
					s[i][j++] = ' ';
				}
				else {
					s[i][j] = ' ';
				}
			}
			if (s[i][j] == '(' && s[i][j + 1] == '*') {
				Delete = true;
				j--;
			}
		}
	}

	for (int i = 0; i < s.size() - 1; i++)
	{
		for (int j = 0; j < s[i].length(); j++)
		{
			if (s[i][j] == '/' && s[i][j + 1] == '/')
				s[i].erase(s[i].find_first_of("/", j), s[i].length());
			if ((int)s[i][j] < 0)
				s[i][j] = ' ';
		}
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
	s.insert(s.begin() + CurrLine, AddSpaces(";BEGIN;", CurrSpaces));
	OperatorsUsed++;
	cout << CurrLine << endl;
	while ((CountLeftSpaces(s[++CurrLine]) == CurrSpaces) && (CurrLine < s.size() - 1));
	cout << CurrSpaces << endl << CountLeftSpaces(s[CurrLine]) << endl;

	if (CurrSpaces > CountLeftSpaces(s[CurrLine]))
	{
		cout << CurrSpaces << endl << CountLeftSpaces(s[CurrLine]) << endl;
		s.insert(s.begin() + CurrLine, AddSpaces(";END;", CurrSpaces));
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
			

		s.insert(s.begin() + CurrLine, AddSpaces(";END;", CurrSpaces));
		OperatorsUsed++;

		for (int i = 0; i < queue.size() - 1; i++)
		{
			cout << "QUEUE: " << queue[i] << endl;
			//if (queue[i] < s.size() - 1)
				//Process(queue[i]);
		}

	}
}

void AddBlocks(int CurrLine)
{
	int CurrSpaces = (CountLeftSpaces(s[CurrLine]));
	int PredSpaces = 0;
	vector <int> q;

	boost::algorithm::trim(s[CurrLine]);
	s[CurrLine] = AddSpaces("{", CurrSpaces) + s[CurrLine];


	for (int i = CurrLine + 1; i < s.size(); i++) {
		if ((CountLeftSpaces(s[i]) < CurrSpaces) || (i == s.size() - 1)) {
			s[i - 1] = s[i - 1] + "}";
			break;
		}
		else if ((CountLeftSpaces(s[i]) > CurrSpaces) && (PredSpaces == CurrSpaces)) {
			q.push_back(i);
		}
		PredSpaces = CountLeftSpaces(s[i]);
	}
	cout << endl << "===" << endl;
	for (int i = 0; i < q.size(); i++) {
		cout << "Q: " << q[i] << " |";
	}
	if (q.size() == 0)
		cout << "EMPTY Q";
	cout << endl << "===" << endl;

	for (int i = 0; i < q.size(); i++) {
		AddBlocks(q[i]);
	}


}

int main()
{
	setlocale(LC_ALL, "rus");
	ifstream file;
	char buf[500];
	string str = "";
	file.open("Program.fs", ios_base::in);
	file.imbue(std::locale(""));
	while (!file.eof()) {
		file.getline(buf, 500);
		s.push_back(buf);
	}

	DeleteComments();
	EraseEmptyStrings();
	AddBlocks(0);

	cout << endl << endl << "====================" << endl;
	for (int i = 0; i < s.size(); i++)
	{
		cout << s[i] << endl;
	}
	cout << "====================" << endl;

	file.close();

	system("pause");
	return 0;
}

	/*
	cout << CurrLine << endl;
	while ((CountLeftSpaces(s[++CurrLine]) == CurrSpaces) && (CurrLine < s.size() - 1));
	cout << CurrSpaces << endl << CountLeftSpaces(s[CurrLine]) << endl;

	if (CurrSpaces > CountLeftSpaces(s[CurrLine]))
	{
		cout << CurrSpaces << endl << CountLeftSpaces(s[CurrLine]) << endl;
		s.insert(s.begin() + CurrLine, AddSpaces(";END;", CurrSpaces));
		OperatorsUsed++;
	}
	else if (CurrSpaces < CountLeftSpaces(s[CurrLine])) {
		//Process(CurrLine);
		vector <int> queue;
		int DefSpaces = 1;
		while ((CountLeftSpaces(s[++CurrLine]) >= CurrSpaces) && (CurrLine < s.size() - 1))
			if ((CountLeftSpaces(s[CurrLine]) > CurrSpaces) && (DefSpaces))
			{
				DefSpaces = CountLeftSpaces(s[CurrLine]);
				queue.push_back(CurrLine + OperatorsUsed);
			}


		s.insert(s.begin() + CurrLine, AddSpaces(";END;", CurrSpaces));
		OperatorsUsed++;

		for (int i = 0; i < queue.size() - 1; i++)
		{
			cout << "QUEUE: " << queue[i] << endl;
			//if (queue[i] < s.size() - 1)
			//Process(queue[i]);
		}

	}*/

	//Process(0);

	//Process(14);
	/*
	enum Token {
	BEGIN, END, IF
	};

	struct Lexem {
	Token tok;
	//string value;
	//Lexem() :tok(BEGIN) {};
	Lexem(Token tk) :tok(tk) {};
	};

	vector <Lexem> v;*/
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


	//cout << CountSpaces(s[i]);
	//cout << s[i] << endl;
	//PredS = CurrS;
	//}


