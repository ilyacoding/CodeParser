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

vector <string> s, l, types, op, opCond, opCondCheck, opPrint, vars, opHolsted;
int HolstedOperators[36];
int OperatorsUsed = 0;
int CLI = 0, CL = 0, cl = 0;
int p = 0, m = 0, cc = 0, t = 0, SPEN = 0;
int opCount = 0, varCount = 0, opFullCount = 0, varFullCount = 0, vocabulary = 0, longitude = 0;
float volume = 0;

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

	opHolsted.push_back("+");
	opHolsted.push_back("-");
	opHolsted.push_back("*");
	opHolsted.push_back("/");
	opHolsted.push_back("%");
	opHolsted.push_back("**");

	opHolsted.push_back("=");
	opHolsted.push_back(">");
	opHolsted.push_back("<");
	opHolsted.push_back(">=");
	opHolsted.push_back("<=");
	opHolsted.push_back("<>");

	opHolsted.push_back("&&&");
	opHolsted.push_back("|||");
	opHolsted.push_back("~~~");
	opHolsted.push_back("^^^");
	opHolsted.push_back("»>");
	opHolsted.push_back("«<");

	opHolsted.push_back("for");
	opHolsted.push_back("while");
	opHolsted.push_back("if");
	opHolsted.push_back("elif");
	opHolsted.push_back("match");
	opHolsted.push_back("|");
	opHolsted.push_back(".");
	opHolsted.push_back(",");
	opHolsted.push_back(":");
	opHolsted.push_back("(");
	opHolsted.push_back("[");
	opHolsted.push_back("<-");

	opHolsted.push_back("||");
	opHolsted.push_back("&&");
	opHolsted.push_back("not");
	opHolsted.push_back("break");
	opHolsted.push_back("continue");
}

class OperandDict {
public:
	vector <string> Operands;
	vector <int> AmountOperands;
	void Add(string str);
	bool IsIn(string str);
	void Print();
};

OperandDict opDict;

bool OperandDict::IsIn(string str)
{
	for (int i = 0; i < Operands.size(); i++)
		if (Operands[i] == str)
			return true;
	return false;
}

void OperandDict::Add(string str)
{
	if (IsIn(str))
	{
		for (int i = 0; i < Operands.size(); i++)
			if (Operands[i] == str) {
				AmountOperands[i]++;
				break;
			}
	}
	else {
		Operands.push_back(str);
		AmountOperands.push_back(0);
	}
}

void OperandDict::Print()
{
	cout.setf(ios::left);
	cout << "___________________________" << endl;
	cout << "| Операнд:     Количество: |" << endl;
	cout << "---------------------------" << endl;


	for (int i = 0; i < Operands.size(); i++)
		cout << "| " << setfill('.') << setw(18) << (Operands[i] + " ") << "| " << setfill(' ') << setw(3) << AmountOperands[i] << " |" << endl;
	cout << "---------------------------" << endl;
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
	l.clear();
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

int IsHolstedLexem(string str)
{
	for (int i = 0; i < opHolsted.size(); i++)
		if (str == opHolsted[i])
			return 1;
	return 0;
}

int GetIndexOfHolsted(string str)
{
	for (int i = 0; i < opHolsted.size(); i++)
		if (str == opHolsted[i])
			return i;
	return 35;
}

int IsCondLexem(string str)
{
	for (int i = 0; i < opCond.size(); i++)
		if (str == opCond[i])
			return 1;
	return 0;
}

bool is_digits(const std::string &str)
{
	return str.find_first_not_of("0123456789") == std::string::npos;
}

vector<string> GetStrLexems(string str)
{
	string buf = "";
	trim(str);
	vector<string> lexems;

	for (int j = 0; j < str.length(); j++)
	{
		while ((isalpha(str[j]) || (str[j] == '.' && is_digits(buf)) || isdigit(str[j])) && (j < str.length())) buf += str[j++];
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

void GetHolstedLexems()
{
	l.clear();
	for (int i = 0; i < s.size() - 1; i++)
	{
		string buf = "";
		string str = s[i];
		//str = boost::algorithm::trim_copy(s[i]);
		trim(str);

		for (int j = 0; j < str.length(); j++)
		{
			while ((isalpha(str[j]) || (str[j] == '.' && is_digits(buf)) || isdigit(str[j])) && (j < str.length())) buf += str[j++];
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

vector <string> GetFuncParams(string str)
{
	vector <string> result;
	trim(str);
	auto x = GetStrLexems(str);
	bool HasType = false;
	bool IsFunc = true;
	int k = 0;
	//return x;
	for (int i = 0; i < types.size(); i++)
		if (x[0] == types[i])
			HasType = true;
	if (!HasType) return result;
	if (x[1] == "mutable") return result;
	if (x[x.size() - 2] != "=") return result;
	// let func ACB C =
	for (int i = 2; i < x.size() - 2; i++)
	{
		result.push_back(x[i]);
	}

	return result;
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
	// Где есть переходы на след уровни
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
			if (LeftAssign == 0 && RightAssign > 0 && !M && !C)
				P = true;
			if (!C && !M && !P)
				T = true;

			cc += (int)C;
			m += (int)M;
			p += (int)P;
			t += (int)T;
			cout.setf(ios::left);
			//cout << "DEBUG: Var: " << setfill(' ') << setw(5) << VName << " Спен: " << setfill(' ') << setw(2) << Spen << "; L:" << LeftAssign << "; R:" << RightAssign << "; C:" << ConditionUsage << endl;
			cout << "Var: " << setfill(' ') << setw(15) << VName << " Спен: " << setfill(' ') << setw(2) << Spen << "; P:" << (int)P << "; M:" << (int)M << "; C:" << (int)C << "; T:" << (int)T << endl;
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

	// Где есть переходы на след уровни
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

	cout << endl << "==================== Метрика Джилба ====================" << endl;
	cout.precision(3);
	cout << "CL: " << CL << endl;
	cout << "cl: " << (float)cl/CL << endl;
	cout << "CLI: " << CLI << endl;
	//auto x = GetStrLexems("            if              a>      field.DICK > n >  2.0         b                       then              ");
	//for (int i = 0; i < x.size(); i++)
	//	cout << x[i] << "_";

	cout << endl << "=================== Метрика Холстеда ====================" << endl << endl;
	cout.precision(6);
	GetHolstedLexems();

	for (int i = 0; i < l.size(); i++)
		HolstedOperators[GetIndexOfHolsted(l[i])] += IsHolstedLexem(l[i]);
	cout << "___________________________" << endl;
	cout << "| Оператор:    Количество: |" << endl;
	cout << "---------------------------" << endl;
	cout.setf(ios::left);
	for (int i = 0; i < opHolsted.size(); i++)
		if (HolstedOperators[i])
		{
			//cout << "Var: " << setfill(' ') << setw(15) << VName << " Спен: " << setfill(' ') << setw(2) << Spen << "; P:" << (int)P << "; M:" << (int)M << "; C:" << (int)C << "; T:" << (int)T << endl;

			cout << "| " << setfill('.') << setw(18) << (opHolsted[i] + " ") << "| " << setfill(' ') << setw(3) << HolstedOperators[i] << " |" << endl;
			opCount++;
			opFullCount += HolstedOperators[i];
		}

	cout << "---------------------------" << endl;
	cout << " Число операторов: " << opCount << endl;
	cout << " Общее число операторов: " << opFullCount << endl;
	cout << "---------------------------" << endl;

	for (int i = 0 ; i < s.size(); i++)
	{
		string VName = GetVarName(s[i]);				
		// BEGIN ПАРСИНГ ПАРАМЕТРОВ
		/*
		auto f = GetFuncParams(s[i]);
		for (int y = 0; y < f.size(); y++)
			if (!opDict.IsIn(f[y]))
			{
				opDict.Add(f[y]);
				for (int j = i + 1; j < s.size(); j++)
				{
					if (!GetVarName(s[j]).length())
					{
						auto x = GetStrLexems(s[j]);
						for (int k = 0; k < x.size(); k++)
						{
							if (x[k] == f[y])
								opDict.Add(f[y]);
						}
					}
				}
			}
		*/
		// END ПАРСИНГ ПАРАМЕТРОВ
		if (VName.length() > 0 && !opDict.IsIn(VName)) {
			opDict.Add(VName);
			for (int j = i + 1; j < s.size(); j++)
			{
				if (!GetVarName(s[j]).length())
				{
					auto x = GetStrLexems(s[j]);
					for (int k = 0; k < x.size(); k++)
					{
						if (x[k] == VName)
							opDict.Add(VName);
					}
				}
			}
		}
	}


	opDict.Print();

	varCount = opDict.AmountOperands.size();
	cout << " Число операндов: " << varCount << endl;
	for (int i = 0; i < opDict.AmountOperands.size(); i++)
		varFullCount += opDict.AmountOperands[i];
	cout << " Общее число операндов: " << varFullCount << endl;
	cout << "---------------------------" << endl << endl;

	vocabulary = varCount + opCount;
	longitude = varFullCount + opFullCount;
	volume = longitude * ((float)log(vocabulary) / log(2));

	cout << "---------Результат----------" << endl;
	cout << " Словарь программы: " << vocabulary << endl;
	cout << " Длина программы: " << longitude << endl;
	cout << " Объём программы: " << volume << endl;
	cout << "----------------------------" << endl;

	/*cout << endl << "==================== Полная метрика Чепина ====================" << endl;
	ProcessVars();
	cout << "Спен = " << (int)SPEN << endl;
	cout << "P: " << p << endl;
	cout << "M: " << m << endl;
	cout << "C: " << cc << endl;
	cout << "T: " << t << endl;
	cout << "Q = " << p << " + 2*" << m << " + 3*" << cc << " + 0,5*" << t << endl;
	cout << "Q = " << (p + 2 * m + 3 * cc + 0.5*t) << endl;*/

	cout << endl;
	system("pause");
	return 0;
}
