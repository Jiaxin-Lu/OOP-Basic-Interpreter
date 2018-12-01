/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "statement.h"
#include "evalstate.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program &program, EvalState &state);
void ExecutedDirectly(string token, string line, EvalState &state);

/* Main program */

int main()
{
	EvalState state;
	Program program;
	while (true)
	{
		try
		{
			processLine(getLine(), program, state);
		}
		catch (ErrorException &ex)
		{
			cerr << "Error: " << ex.getMessage() << endl;
		}
	}
	return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program &program, EvalState &state)
{
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(line);
	string firsttoken = scanner.nextToken();
	if (firsttoken == "LET" || firsttoken == "PRINT" || firsttoken == "INPUT")
	{
		ExecutedDirectly(firsttoken, line, state);
		return;
	}
	if (scanner.getTokenType(firsttoken) != NUMBER)
	{
		if (firsttoken == "RUN")
		{
			program.RUN_(state);
			return;
		}
		else if (firsttoken == "LIST")
		{
			program.LIST_();
			return;
		}
		else if (firsttoken == "HELP")
		{
			program.HELP_();
			return;
		}
		else if (firsttoken == "QUIT")
		{
			exit(0);
			return;
		}
		else if (firsttoken == "CLEAR")
		{
			program.CLEAR_(program, state);
			return;
		}
		else
		{
			cout << "SYNTAX ERROR1" << endl;
			return;
		}
	}
	int linenumber = -1;
	istringstream stream(firsttoken);
	stream >> linenumber >> ws;
	if (stream.fail() || !stream.eof())
	{
		cout << "SYNTAX ERROR2" << endl;
		return;
	}
	if (linenumber < 0)
	{
		cout << "SYNTAX ERROR3" << endl;
		return;
	}
	//cout << "ln:" << linenumber << endl;
	if (!scanner.hasMoreTokens())
	{
		bool flag_usedLineNumber;
		if (program.usedLineNumber(linenumber, flag_usedLineNumber))
			program.removeSourceLine(linenumber);
	}
	else
	{
		string secondtoken = scanner.nextToken();
		bool flag_secondtoken = true;
		Statement *st_tmp = nullptr;
		if (secondtoken == "REM")
		{
			st_tmp = new REMST();
			//cout << "INREM" << endl;
		}
		else if (secondtoken == "INPUT")
		{
			st_tmp = new INPUTST(line);
			flag_secondtoken = st_tmp->CheckError();
		}
		else if (secondtoken == "LET")
		{
			st_tmp = new LETST(line);
			flag_secondtoken = st_tmp->CheckError();
		}
		else if (secondtoken == "PRINT")
		{
			st_tmp = new PRINTST(line);
			flag_secondtoken = st_tmp->CheckError();
		}
		else if (secondtoken == "END")
		{
			st_tmp = new ENDST(line);
			flag_secondtoken = st_tmp->CheckError();
		}
		else if (secondtoken == "GOTO")
		{
			st_tmp = new GOTOST(line);
			flag_secondtoken = st_tmp->CheckError();
			//cout<<"BASIC : TOLINENUMBER" << st_tmp->tolineNumber <<endl;
		}
		else if (secondtoken == "IF")
		{
			st_tmp = new IFST(line);
			flag_secondtoken = st_tmp->CheckError();
		}
		else
		{
			delete st_tmp;
			cout << "SYNTAX ERROR4" << endl;
			return;
		}
		if (flag_secondtoken == false)
		{
			delete st_tmp;
			cout << "SYNTAX ERROR5" << endl;
			return;
		}
		program.addSourceLine(linenumber, line);
		program.setParsedStatement(linenumber, st_tmp);
		//delete st_tmp;
	}
	return;
}

void ExecutedDirectly(string token, string line, EvalState &state)
{
	Statement *st_tmp = nullptr;
	if (token == "LET")
	{
		st_tmp = new LETST(line);
		if (!st_tmp->CheckError())
		{
			delete st_tmp;
			return;
		}
		else
		{
			st_tmp->execute_direct(state);
		}
	}
	else if (token == "INPUT")
	{
		st_tmp = new INPUTST(line);
		if (!st_tmp->CheckError())
		{
			delete st_tmp;
			return;
		}
		else
		{
			st_tmp->execute(state);
		}
	}
	else if (token == "PRINT")
	{
		st_tmp = new PRINTST(line);
		if (!st_tmp->CheckError())
		{
			delete st_tmp;
			return;
		}
		else
		{
			st_tmp->execute(state);
		}
	}
	delete st_tmp;
	return;
}
