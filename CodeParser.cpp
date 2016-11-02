// CodeParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "fstream"
#include "string"
#include "vector"
#include <algorithm> 
#include <functional> 
#include <sstream>
#include <cctype>
#include <locale>
#include <iostream>
#include <iomanip>

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

vector <string> s, l, types, op, opCond, opCondCheck, opPrint, vars;
int OperatorsUsed = 0;
int CLI = 0, CL = 0, cl = 0;
int p = 0, m = 0, cc = 0, t = 0, SPEN = 0;

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
	
	op.push_back("member");
	op.push_back("override");
	op.push_back("lock");
	op.push_back("type");
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
	op.push_back("elif");
	op.push_back("assert");
	op.push_back("match");
	op.push_back("|");

	opCond.push_back("for");
	opCond.push_back("while");
	opCond.push_back("if");
	opCond.push_back("elif");
	opCond.push_back("match");
	opCond.push_back("|");

	opCondCheck.push_back("for");
	opCondCheck.push_back("while");
	opCondCheck.push_back("if");
	opCondCheck.push_back("elif");
	opCondCheck.push_back("match");

	opPrint.push_back("printfn");
	opPrint.push_back("printf");
	opPrint.push_back("sprintf");

	types.push_back("let");
	types.push_back("bool");
	types.push_back("byte");
	types.push_back("sbyte");
	types.push_back("int16");
	types.push_back("uint16");
	types.push_back("int");
	types.push_back("uint32");
	types.push_back("int64");
	types.push_back("uint64");
	types.push_back("nativeint");
	types.push_back("unativeint");
	types.push_back("char");
	types.push_back("string");
	types.push_back("decimal");
	types.push_back("float32");
	types.push_back("single");
	types.push_back("float");
	types.push_back("double");
}

string IntToStr(int val)
{
	stringstream ss;
	ss << val;

	return ss.str();
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
		string str = s[i];
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
		l.push_back(";");
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

vector<string> GetStrLexems(string str)
{
	string buf = "";
	trim(str);
	vector<string> lexems;

	for (int j = 0; j < str.length(); j++)
	{
		while ((isalpha(str[j])/* || str[j] == '.'*/ || isdigit(str[j])) && (j < str.length())) buf += str[j++];
		if (buf.length() > 0) {
			trim(buf);
			lexems.push_back(buf);
			buf = "";
		}

		while ((!isspace(str[j]) && !isalpha(str[j]) && !isdigit(str[j])) && (j < str.length())) buf += str[j++];
		if (buf.length() > 0) {
			trim(buf);
			lexems.push_back(buf);
			buf = "";
		}
		if (isalpha(str[j]) || isdigit(str[j])) j--;
	}
	lexems.push_back(";");
	return lexems;
}

int IsConditionUsage(string str, string VarName)
{
	trim(str);
	trim(VarName);
	auto lexems = GetStrLexems(str);
	bool IsCond = false;
	for (int i = 0; i < opCondCheck.size(); i++) 
		if (opCondCheck[i] == lexems[0])
			IsCond = true;
	if (!IsCond) return 0;

	int counter = 0;
	for (int i = 1; i < lexems.size(); i++)
		if (lexems[i] == VarName)
			counter++;
	//cout << "<" << str << "_" << counter << ">" << endl;
	return counter;
}

int IsFuncPrint(string str, string VarName)
{
	trim(str);
	trim(VarName);
	auto lexems = GetStrLexems(str);
	int counter = 0;

	bool IsPrint = false;
	for (int i = 0; i < opPrint.size(); i++)
		if (opPrint[i] == lexems[0])
			IsPrint = true;
	if (!IsPrint) return counter;

	for (int i = 1; i < lexems.size(); i++)
		if (lexems[i] == VarName)
			counter++;
	//cout << VarName << " " << counter << endl;
	return counter;
}

int IsRightAssign(string str, string VarName)
{
	trim(str);
	trim(VarName);
	auto lexems = GetStrLexems(str);
	int counter = 0, i = 0;

	bool IsAssign = false;
	for (int i = 0; i < lexems.size(); i++) 
		if (lexems[i] == "<-")
			IsAssign = true;
	if (!IsAssign) return counter;

	//for (int i = 0; i < lexems.size(); i++) {
	while (lexems[i] != "<-" && i < lexems.size()) i++;
	
	if (i >= lexems.size()) 
		return counter;
	else
		i++;

	while (lexems[i] != "<-" && i < lexems.size()) {
		if (lexems[i] == VarName)
			counter++;
		i++;
		if (i >= lexems.size()) return counter;
	}
	//}
	return counter;
}

int IsLeftAssign(string str, string VarName)
{
	trim(str);
	trim(VarName);
	auto lexems = GetStrLexems(str);
	int counter = 0, i = 0;

	bool IsAssign = false;
	for (int i = 0; i < lexems.size(); i++)
		if (lexems[i] == "<-")
			IsAssign = true;
	if (!IsAssign) return counter;

	//for (int i = 0; i < lexems.size(); i++) {
	while (lexems[i] != "<-" && i < lexems.size()) {
		if (lexems[i] == VarName)
			counter++;
		i++;
		if (i >= lexems.size()) break;
	}
	//}
	return counter;
}

string GetVarName(string str)
{
	trim(str);
	int k = 0;
	string buf = "";
	string VarName = "";
	bool HasType = false;
	bool IsFunc = true;
	while (!isspace(str[k]) && k < str.size()) buf += str[k++];
	for (int i = 0; i < types.size(); i++)
		if (buf == types[i])
			HasType = true;
	if (!HasType) return "";
	if (str[str.size() - 1] == '=') return "";
	
	buf = "";
	while (isspace(str[k])) k++;
	while (!isspace(str[k])) buf += str[k++];
	if (buf == "mutable") {
		buf = "";
		while (isspace(str[k])) k++;
		while (!isspace(str[k]) && k < str.size()) VarName += str[k++];
	}
	else {
		VarName = buf;
	}

	return VarName;
}

void MaxCLI(int val)
{
	if (val > CLI)
		CLI = val;
}

int Spen, LeftAssign, RightAssign, ConditionUsage;
string VarName;

bool IsUsedLine(vector<int> &usedLines, int Line) {
	for (int i = 0; i < usedLines.size(); i++)
		if (Line == usedLines[i])
			return true;
	return false;
}

void GoDepth(int CurrLine)
{
	// CurrLvl = now the max cond depth
	int CurrSpaces = (CountLeftSpaces(s[CurrLine])); // Normal spaces on current lvl
	int PredSpaces = 0;

	vector <int> q, usedLines;
	q.push_back(CurrLine + 1);
	//CurrLine++;
	// ��� ���� �������� �� ���� ������
	for (int i = CurrLine; i < s.size() - 1; i++) {
		//while ((CountLeftSpaces(s[i]) == CountLeftSpaces(s[CurrLine])) && (i < (s.size() - 1))) i++;
		if (CountLeftSpaces(s[i]) > CountLeftSpaces(s[CurrLine]))
			q.push_back(i);
		while ((CountLeftSpaces(s[i]) > CountLeftSpaces(s[CurrLine])) && (i < (s.size() - 1))) i++;
		if (CountLeftSpaces(s[i]) < CountLeftSpaces(s[CurrLine])) break;
	}

	int k = q[q.size() - 1] - 1;
	int j = k + 1;
	while (CountLeftSpaces(s[CurrLine]) != CountLeftSpaces(s[j]) && j < s.size() - 1)
		if (CountLeftSpaces(s[CurrLine]) >= CountLeftSpaces(s[j]) && j < s.size() - 1)
			break;
		else
			j++;

	if (CountLeftSpaces(s[CurrLine]) == CountLeftSpaces(s[j]) && j < s.size() - 1)
		q.push_back(j);
	else
		q.push_back(-1);


	j = 0;
	for (int i = 0; i < q.size(); i++) {
		if (i < q.size() - 1) {
			j = q[i] - 1;
		} else {
			j = q[i];
		}
		if ((j < 0) || (j >= s.size() - 1)) break;
		while ((CountLeftSpaces(s[j]) == CountLeftSpaces(s[CurrLine])) && (!IsUsedLine(usedLines, j))) {
			if (GetVarName(s[j]) == VarName) {
				q.erase(q.begin() + i);
				break;
			}
		// Iterate here
			usedLines.push_back(j);
			LeftAssign += IsLeftAssign(s[j], VarName);
			RightAssign += IsRightAssign(s[j], VarName);
			RightAssign += IsFuncPrint(s[j], VarName);
			ConditionUsage += IsConditionUsage(s[j], VarName);
		// Iterate here
		
			j++;
		}
	}
	
	/*cout << endl << "===" << endl;
	for (int i = 0; i < q.size(); i++) {
		cout << "Q: " << q[i] << "| ";
	}

	if (q.size() - 2 == 0)
	cout << "EMPTY Q";*/
	
	for (int i = 1; i < q.size() - 1; i++) {
		GoDepth(q[i]);
	}
}

void ProcessVars()
{
	bool P, M, C, T;

	for (int i = 0; i < s.size(); i++)
	{
		P = M = C = T = false;
		string VName = GetVarName(s[i]);
		if (VName.length() > 0) {
			VarName = VName;
			//if (VName != "rez") continue;
			LeftAssign = 0;
			RightAssign = 0;
			ConditionUsage = 0; 
			Spen = 0;
			GoDepth(i + 1);
			Spen = LeftAssign + RightAssign + ConditionUsage;
			SPEN += Spen;
			if (ConditionUsage > 0)
				C = true;
			if (LeftAssign > 0 && RightAssign > 0 && !C)
				M = true;
			if (LeftAssign == 0 && !M)
				P = true;
			if (!C && !M && !P)
				T = true;

			cc += (int)C;
			m += (int)M;
			p += (int)P;
			t += (int)T;
			cout.setf(ios::left);
			//cout << "DEBUG: Var: " << setfill(' ') << setw(5) << VName << " ����: " << setfill(' ') << setw(2) << Spen << "; L:" << LeftAssign << "; R:" << RightAssign << "; C:" << ConditionUsage << endl;
			cout << "Var: " << setfill(' ') << setw(15) << VName << " ����: " << setfill(' ') << setw(2) << Spen << "; P:" << (int)P << "; M:" << (int)M << "; C:" << (int)C << "; T:" << (int)T << endl;
		}
	}
}

void CalcDepth(int CurrLine, int CurrLvl)
{
	// CurrLvl = now the max cond depth
	int CurrSpaces = (CountLeftSpaces(s[CurrLine])); // Normal spaces on current lvl
	int PredSpaces = 0;
	int CurrLvlEnter = CurrLvl;

	vector <int> q;
	vector <int> qS;

	// ��� ���� �������� �� ���� ������
	for (int i = CurrLine; i < s.size() - 1; i++) {
		if (CountLeftSpaces(s[i]) > CountLeftSpaces(s[CurrLine])) 
			q.push_back(i);
		while ((CountLeftSpaces(s[i]) > CountLeftSpaces(s[CurrLine])) && (i < (s.size() - 1))) i++;
		if (CountLeftSpaces(s[i]) < CountLeftSpaces(s[CurrLine])) break;
	}

	for (int i = CurrLine; i < s.size(); i++) {
		if (s[i].find("match", 0) != string::npos) {
			int CountCase = 0;
			i++;
			string str = s[i];
			while (trim(str)[0] == '|') {
				CountCase++;
				str = s[++i];
			}

			/*if (CountCase == 1 || CountCase == 2) {
				CountCase = 0;
			} else if (CountCase == 3) {
				CountCase = 1;
			} else {
				CountCase -= 2;
			}
			if (CurrLvl > 0) {
				CountCase--;
			}*/
			if (CountCase == 1 || CountCase == 2) {
				CountCase = 1;
				if (CurrLvl > 0)
					CountCase--;
			} else {
				CountCase -= 2;
			}
			MaxCLI(CurrLvl + CountCase);
			CurrLvlEnter = CurrLvl;
			//cout << "MATCH" << endl;
		}
	}

	for (int i = 0; i < q.size(); i++) {
		int Line = q[i] - 1;
		if ((s[Line].find("if", 0) != string::npos) && (s[Line].find("elif", 0) == string::npos)) {
			CurrLvlEnter = CurrLvl;
			MaxCLI(CurrLvlEnter++);
			qS.push_back(CurrLvlEnter);
			//cout << "IF" << endl;
		} else if (s[Line].find("while", 0) != string::npos) {
			CurrLvlEnter = CurrLvl;
			MaxCLI(CurrLvlEnter++);
			qS.push_back(CurrLvlEnter);
			//cout << "WHILE" << endl;
		} else if (s[Line].find("for", 0) != string::npos) {
			CurrLvlEnter = CurrLvl;
			MaxCLI(CurrLvlEnter++);
			qS.push_back(CurrLvlEnter);
			//cout << "FOR" << endl;
		} else if (s[Line].find("elif", 0) != string::npos) {
			//if (ProcessElif) 
				MaxCLI(CurrLvlEnter++);
			qS.push_back(CurrLvlEnter);
			//cout << "ELIF" << endl;
		} else if (s[Line].find("else", 0) != string::npos) {
			MaxCLI(CurrLvlEnter - 1);
			qS.push_back(CurrLvlEnter);
			CurrLvlEnter = CurrLvl;
			//cout << "ELSE" << endl;
		} else {
			CurrLvlEnter = CurrLvl;
			qS.push_back(CurrLvl);
			//cout << "FUNC" << endl;
		}

	}
	
	/*cout << endl << "===" << endl;
	cout << "LEVEL: " << CurrLvl << endl;
	for (int i = 0; i < q.size(); i++) {
		cout << "Q: " << q[i] << "; CLI: " << qS[i] << "|";
	}

	if (q.size() == 0)
		cout << "EMPTY Q";*/
	
	for (int i = 0; i < q.size(); i++) {
		CalcDepth(q[i], qS[i]);
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
	char c;
	string FileName;
	cout << "a) ProgramBig " << endl
	     << "b) ProgramTest " << endl
	     << "c) ProgramBlock " << endl
		 << "def: ProgramDebug " << endl << endl;
	cout << "Select: ";
	cin >> c;
	switch (c) {
		case 'a':
			FileName = "ProgramBig";
			break;
		case 'b':
			FileName = "ProgramTest";
			break;
		case 'c':
			FileName = "ProgramBlock";
			break;
		default:
			FileName = "ProgramDebug";
			break;
	}
	file.open(("test/" + FileName + ".fs").c_str(), ios_base::in);
	file.imbue(std::locale(""));
	
	while (!file.eof()) {
		file.getline(buf, 500);
		s.push_back(buf);
	}

	file.close();

	DeleteComments();
	DeleteStrings();
	EraseEmptyStrings();



	GetLexems();
	//();
	CalcDepth(0, 0);

	cout << "==================== CODE ====================" << endl;
	for (int i = 0; i < s.size(); i++)
		cout << s[i] << endl;
	cout << "==================== /CODE ====================" << endl;

	for (int i = 0; i < l.size() - 1; i++)
	{
		//cout << l[i] << ":" << IsLexem(l[i]) << ":" << IsCondLexem(l[i]) << endl;
		CL += IsLexem(l[i]);
		cl += IsCondLexem(l[i]);
	}
	
	for (int i = 0; i < vars.size(); i++) {
		cout << vars[i] << " _ ";
	}

	cout << endl << "==================== ������� ������ ====================" << endl;
	cout.precision(3);
	cout << "CL: " << CL << endl;
	cout << "cl: " << (float)cl/CL << endl;
	cout << "CLI: " << CLI << endl;
	cout << endl << "==================== ������� ������ ====================" << endl;
	ProcessVars();
	cout << "���� = " << (int)SPEN << endl;
	cout << "Q = " << p << " + 2*" << m << " + 3*" << cc << " + 0,5*" << t << endl;
	cout << "Q = " << (p + 2 * m + 3 * cc + 0.5*t) << endl;

	cout << endl;
	system("pause");
	return 0;
}
