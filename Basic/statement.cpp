/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "parser.h"
#include "../StanfordCPPLib/strlib.h"
#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/tokenscanner.h"
using namespace std;

#define DIVIDEZERO -214748

/* Implementation of the Statement class */

Statement::Statement()
{
	/* Empty */
}

Statement::~Statement()
{
	/* Empty */
}

bool Statement::CheckKeyWord(string str)
{
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(str);
	while (scanner.hasMoreTokens())
	{
		string tmp = scanner.nextToken();
		if (tmp == "REM" || tmp == "LET" || tmp == "PRINT" || tmp == "INPUT" || tmp == "END" || tmp == "IF" || tmp == "THEN" || tmp == "GOTO")
		{
			cout << "SYNTAX ERROR" << endl;
			return false;
		}
	}
	return true;
}

bool Statement::CheckVar(string str, EvalState &state)
{
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(str);
	while (scanner.hasMoreTokens())
	{
		string tmp = scanner.nextToken();
		TokenType tmptype = scanner.getTokenType(tmp);
		if (tmptype == WORD && !(state.isDefined(tmp)))
		{
			cout << "VARIABLE NOT DEFINED" << endl;
			return false;
		}
	}
	return true;
}
void REMST::execute_()
{
	return;
}

StatementTYPE REMST::getTYPE()
{
	return remst;
}

void LETST::execute(EvalState &state)
{
	if (!Statement::CheckVar(expstr, state))
		return;
	try
	{
		int val = exp->eval(state);
		state.setValue(var, val);
	}
	catch (...)
	{
		return;
	}
	return;
}
void LETST::execute_direct(EvalState &state)
{
	if (!Statement::CheckVar(expstr, state))
		return;
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(expstr);
	string token = scanner.nextToken();
	scanner.saveToken(token);
	TokenType type = scanner.getTokenType(token);
	if (type != WORD && type != NUMBER)
	{
		cout << "VARIABLE NOT DEFINED" << endl;
		return;
	}
	try
	{
		int val = exp->eval(state);
		state.setValue(var, val);
	}
	catch (...)
	{
		return;
	}
	return;
}
StatementTYPE LETST::getTYPE()
{
	return letst;
}
bool LETST::CheckError()
{
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(line);
	string firsttoken = scanner.nextToken();
	if (scanner.getTokenType(firsttoken) == NUMBER)
		string tmp = scanner.nextToken();
	if (!scanner.hasMoreTokens())
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	firsttoken = scanner.nextToken();
	if (!scanner.hasMoreTokens())
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	string op = scanner.nextToken();
	if (op != "=")
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	if (!scanner.hasMoreTokens())
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	TokenScanner tmpscanner;
	while (scanner.hasMoreTokens())
	{
		string nexttoken = scanner.nextToken();
		TokenType nexttype = scanner.getTokenType(nexttoken);
		if (nexttype != NUMBER && nexttype != WORD && nexttype != OPERATOR)
		{
			cout << "SYNTAX ERROR" << endl;
			return false;
		}
		else
		{
			expstr.append(nexttoken);
			expstr.append(" ");
			tmpscanner.saveToken(nexttoken);
		}
	}
	if (!Statement::CheckKeyWord(firsttoken))
		return false;
	if (!Statement::CheckKeyWord(expstr))
		return false;
	TokenScanner orderedscanner;
	while (tmpscanner.hasMoreTokens())
		orderedscanner.saveToken(tmpscanner.nextToken());
	exp = parseExp(orderedscanner);
	if (exp == nullptr)
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	var = firsttoken;
	return true;
}

void PRINTST::execute(EvalState &state)
{
	if (!Statement::CheckVar(expstr, state))
	{
		return;
	}
	try
	{
		int val = exp->eval(state);
		if (val == DIVIDEZERO)
			return;
		cout << val << endl;
	}
	catch (...)
	{
		return;
	}
	return;
}
StatementTYPE PRINTST::getTYPE()
{
	return printst;
}
bool PRINTST::CheckError()
{
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(line);
	string firsttoken = scanner.nextToken();
	if (scanner.getTokenType(firsttoken) == NUMBER)
		string tmp = scanner.nextToken();
	if (!scanner.hasMoreTokens())
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	TokenScanner tmpscanner;
	while (scanner.hasMoreTokens())
	{
		string tmptoken = scanner.nextToken();
		expstr.append(tmptoken);
		expstr.append(" ");
		tmpscanner.saveToken(tmptoken);
	}
	if (!Statement::CheckKeyWord(expstr))
	{
		return false;
	}
	TokenScanner orderedscanner;
	while (tmpscanner.hasMoreTokens())
		orderedscanner.saveToken(tmpscanner.nextToken());
	exp = parseExp(orderedscanner);
	if (exp == nullptr)
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	return true;
}
int INPUTST::inputInteger(string prompt)
{
	int value;
	string line;
	while (true)
	{
		cout << prompt;
		getline(cin, line);
		istringstream stream(line);
		stream >> value >> ws;
		if (!stream.fail() && stream.eof())
			break;
		cout << "INVALID NUMBER" << endl;
	}
	return value;
}
void INPUTST::execute(EvalState &state)
{
	int val = inputInteger(" ? ");
	state.setValue(var, val);
	return;
}
StatementTYPE INPUTST::getTYPE()
{
	return inputst;
}
bool INPUTST::CheckError()
{
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(line);
	string firsttoken = scanner.nextToken();
	if (scanner.getTokenType(firsttoken) == NUMBER)
		string tmp = scanner.nextToken();
	if (!scanner.hasMoreTokens())
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	string nexttoken = scanner.nextToken();
	if (!CheckKeyWord(nexttoken))
	{
		return false;
	}
	if (scanner.hasMoreTokens())
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	if (scanner.getTokenType(nexttoken) == WORD)
	{
		var = nexttoken;
		return true;
	}
	else
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
}
void ENDST::execute_()
{
	return;
}
StatementTYPE ENDST::getTYPE()
{
	return endst;
}
bool ENDST::CheckError()
{
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(line);
	string firsttoken = scanner.nextToken();
	if (scanner.getTokenType(firsttoken) == NUMBER)
		string tmp = scanner.nextToken();
	if (scanner.hasMoreTokens())
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	return true;
}
int GOTOST::execute__(EvalState &state)
{
	//cout << "in execute " << tolineNumber << endl;
	return tolineNumber;
}
StatementTYPE GOTOST::getTYPE()
{
	return gotost;
}
bool GOTOST::CheckError()
{
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(line);
	string firsttoken = scanner.nextToken();
	if (scanner.getTokenType(firsttoken) == NUMBER)
		string tmp = scanner.nextToken();
	if (!scanner.hasMoreTokens())
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	string nexttoken = scanner.nextToken();
	if (scanner.hasMoreTokens())
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	if (scanner.getTokenType(nexttoken) != NUMBER)
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	int num = -1;
	istringstream stream(nexttoken);
	stream >> num >> ws;
	tolineNumber = num;
	//cout << "STATEMENT : TOLINENUMBER" << tolineNumber << endl;
	if (stream.fail() || !stream.eof())
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	if (tolineNumber < 0)
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	return true;
}
int IFST::execute__(EvalState &state)
{
	if (!Statement::CheckVar(exp1str, state))
		return -2;
	if (!Statement::CheckVar(exp2str, state))
		return -2;
	try
	{
		int value1 = exp1->eval(state);
		if (value1 == DIVIDEZERO)
			return -2;
		int value2 = exp2->eval(state);
		if (value2 == DIVIDEZERO)
			return -2;
		if (op == "=")
		{
			if (value1 == value2)
				return tolineNumber;
			else
				return -1;
		}
		else if (op == "<")
		{
			if (value1 < value2)
				return tolineNumber;
			else
				return -1;
		}
		else if (op == ">")
		{
			if (value1 > value2)
				return tolineNumber;
			else
				return -1;
		}
	}
	catch (...)
	{
		return -2;
	}
	return -2;
}
StatementTYPE IFST::getTYPE()
{
	return ifst;
}
bool IFST::CheckError()
{
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(line);
	string firsttoken = scanner.nextToken();
	if (scanner.getTokenType(firsttoken) == NUMBER)
		string tmp = scanner.nextToken();
	if (!scanner.hasMoreTokens())
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	TokenScanner tmpscanner1;
	while (scanner.hasMoreTokens())
	{
		string nexttoken = scanner.nextToken();
		if (nexttoken == "=" || nexttoken == "<" || nexttoken == ">")
		{
			op = nexttoken;
			break;
		}
		exp1str.append(nexttoken);
		exp1str.append(" ");
		tmpscanner1.saveToken(nexttoken);
	}
	if (!scanner.hasMoreTokens())
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	TokenScanner tmpscanner2;
	while (scanner.hasMoreTokens())
	{
		string nexttoken = scanner.nextToken();
		if (nexttoken == "THEN")
		{
			break;
		}
		exp2str.append(nexttoken);
		exp2str.append(" ");
		tmpscanner2.saveToken(nexttoken);
	}
	if (!scanner.hasMoreTokens())
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	string nexttoken = scanner.nextToken();
	if (scanner.hasMoreTokens())
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	if (scanner.getTokenType(nexttoken) != NUMBER)
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	istringstream stream(nexttoken);
	stream >> tolineNumber >> ws;
	if (tolineNumber < 0)
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	if (!Statement::CheckKeyWord(exp1str))
		return false;
	if (!Statement::CheckKeyWord(exp2str))
		return false;

	TokenScanner orderedscanner1;
	while (tmpscanner1.hasMoreTokens())
		orderedscanner1.saveToken(tmpscanner1.nextToken());
	exp1 = parseExp(orderedscanner1);
	if (exp1 == nullptr)
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}

	TokenScanner orderedscanner2;
	while (tmpscanner2.hasMoreTokens())
		orderedscanner2.saveToken(tmpscanner2.nextToken());
	exp2 = parseExp(orderedscanner2);
	if (exp2 == nullptr)
	{
		cout << "SYNTAX ERROR" << endl;
		return false;
	}
	return true;
}
