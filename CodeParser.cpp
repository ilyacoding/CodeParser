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
//#include "boost/algorithm/string.hpp"

// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

using namespace std;

vector <string> s, l, op, opCond;
int OperatorsUsed = 0, CLI = 0, CL = 0, cl = 0;

void InitLexems()
{
	op.push_back("+");
	op.push_back("-");
	op.push_back("/");
	op.push_back("*");
	op.push_back("%");
	op.push_back("**");

	op.push_back("=");
	op.push_back(">");
	op.push_back("<");
	op.push_back(">=");
	op.push_back("<=");
	op.push_back("<>");
	op.push_back("compare");

	op.push_back("&&&");
	op.push_back("|||");
	op.push_back("^^^");
	op.push_back("~~~");
	op.push_back("<<<");
	op.push_back(">>>");

	op.push_back("not");
	op.push_back("||");
	op.push_back("&&");

	op.push_back(":=");
	op.push_back("<-");
	op.push_back("let");
	op.push_back("printfn");
	op.push_back("printf");
	op.push_back("sprintf");

	/*op.push_back("abs");
	op.push_back("ceil");
	op.push_back("exp");
	op.push_back("floor");
	op.push_back("sign");
	op.push_back("log");
	op.push_back("log10");
	op.push_back("sqrt");
	op.push_back("cos");
	op.push_back("sin");
	op.push_back("tan");
	op.push_back("pown");*/

	op.push_back("for");
	op.push_back("while");
	op.push_back("if");
	op.push_back("assert");
	op.push_back("match");

	opCond.push_back("for");
	opCond.push_back("while");
	opCond.push_back("if");
	opCond.push_back("assert");
	opCond.push_back("match");
	opCond.push_back("|");
}

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

void DeleteStrings()
{
	bool Delete = false;
	for (int i = 0; i < s.size() - 1; i++)
	{
		for (int j = 0; j < s[i].length(); j++)
		{
			if (Delete) {
				if (s[i][j] == '"') {
					Delete = false;
					s[i][j++] = ' ';
				}
				else {
					s[i][j] = ' ';
				}
			}
			if (s[i][j] == '"') {
				Delete = true;
				s[i][j] = ' ';
			}
		}
	}
}

void EraseEmptyStrings()
{
	for (int i = 0; i < s.size() - 1; i++)
	{
		string str = s[i];
		rtrim(s[i]);
		trim(str);
		//boost::algorithm::trim_right(s[i]);
		//boost::algorithm::trim(str);
		if (str.length() < 1)
		{
			s.erase(s.begin() + i);
			i--;
		}
	}
}

void GetLexems()
{
	for (int i = 0; i < s.size() - 1; i++)
	{
		string buf = "";
		string str;
		str = s[i];
		//str = boost::algorithm::trim_copy(s[i]);
		trim(str);

		for (int j = 0; j < str.length(); j++)
		{
			while ((isalpha(str[j]) || str[j] == '.' || isdigit(str[j])) && (j < str.length())) buf += str[j++];
			if (buf.length() > 0) {
				//boost::algorithm::trim(buf);
				trim(buf);
				l.push_back(buf);
				buf = "";
			}

			while ((!isspace(str[j]) && !isalpha(str[j]) && !isdigit(str[j])) && (j < str.length())) buf += str[j++];
			if (buf.length() > 0) {
				//boost::algorithm::trim(buf);
				trim(buf);
				l.push_back(buf);
				buf = "";
			}
			if (isalpha(str[j]) || isdigit(str[j])) j--;
		}
	}
}

int IsLexem(string str) 
{
	for (int i = 0; i < op.size(); i++)
		if (str == op[i])
			return 1;
	return 0;
}

int IsCondLexem(string str)
{
	for (int i = 0; i < opCond.size(); i++)
		if (str == opCond[i])
			return 1;
	return 0;
}

void CalcDepth(int CurrLine, int CurrLvl)
{
	int CurrSpaces = (CountLeftSpaces(s[CurrLine]));
	int PredSpaces = 0;
	int CurrLvlEnter = CurrLvl;
	vector <int> q;
	vector <int> qS;

	for (int i = CurrLine; i < s.size(); i++) {
		// Process
		if (s[i].find("match", 0) != string::npos) {
			int CountCase = 0;
			i++;
			string str = "";
			str = s[i];
			//while (boost::algorithm::trim_copy(s[i])[0] == '|') {
			while (trim(str)[0] == '|') {
				CountCase++;
				//i++;
				str = s[++i];
			}
			if (CurrLvl + CountCase - 2 > CLI) {
				CLI = CurrLvl + CountCase - 2;
			}
		}

		if (((s[i].find("if ", 0) != string::npos) || (s[i].find("assert ", 0) != string::npos) || (s[i].find("for ", 0) != string::npos) || (s[i].find("while ", 0) != string::npos)) && (CountLeftSpaces(s[i]) == CurrSpaces)) {
			if (CurrLvl > CLI) {
				CLI = CurrLvl;
			}
		}
		// Process

		if ((CountLeftSpaces(s[i]) < CurrSpaces) || (i == s.size() - 1)) {
			break;
		} else if ((CountLeftSpaces(s[i]) > CurrSpaces) && (PredSpaces == CurrSpaces)) {
			q.push_back(i);
			if (s[i - 1].find("elif ", 0) != string::npos) {
				qS.push_back(++CurrLvlEnter);
				cout << "FOUND ELIF" << CurrLvlEnter << endl;
				if (CurrLvlEnter > CLI) {
					CLI = CurrLvlEnter;
				}
			}
			else {
				qS.push_back(CurrLvlEnter);
			}
		}
		PredSpaces = CountLeftSpaces(s[i]);
	}

	for (int i = 0; i < q.size(); i++) {
		CalcDepth(q[i], qS[i] + 1);
	}


}

int main()
{
	setlocale(LC_ALL, "rus");
	InitLexems();

	ifstream file;
	int CondLexems = 0;
	char buf[500];
	string str = "";
	file.open("Program.fs", ios_base::in);
	file.imbue(std::locale(""));
	while (!file.eof()) {
		file.getline(buf, 500);
		s.push_back(buf);
	}
	file.close();

	DeleteComments();
	DeleteStrings();
	EraseEmptyStrings();

	cout  << "==================== CODE ====================" << endl;
	for (int i = 0; i < s.size(); i++)
		cout << s[i] << endl;
	cout << "==================== /CODE ====================" << endl;

	GetLexems();
	CalcDepth(0, 0);

	

	for (int i = 0; i < l.size() - 1; i++)
	{
		cout << l[i] << ":" << IsLexem(l[i]) << ":" << IsCondLexem(l[i]) << endl;
		CL += IsLexem(l[i]);
		cl += IsCondLexem(l[i]);
	}
	
	cout << endl << "==================== RESULT ====================" << endl;
	cout.precision(2);
	cout << "CL: " << CL << endl;
	cout << "cl: " << (float)cl/CL << endl;
	cout << "CLI: " << CLI << endl;
	
	system("pause");
	return 0;
}
