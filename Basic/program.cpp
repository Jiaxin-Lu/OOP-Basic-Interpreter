/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
using namespace std;

Program::Program()
{
	FIRSTLINE = nullptr;
	// Replace this stub with your own code
}

Program::~Program()
{
	while (FIRSTLINE != nullptr)
	{
		LINE *tmp = FIRSTLINE;
		FIRSTLINE = FIRSTLINE->ne;
		delete tmp;
	}
	return;
	// Replace this stub with your own code
}

void Program::clear()
{
	while (FIRSTLINE != nullptr)
	{
		LINE *tmp = FIRSTLINE;
		FIRSTLINE = FIRSTLINE->ne;
		delete tmp;
	}
	return;
	// Replace this stub with your own code
}

void Program::addSourceLine(int lineNumber, string line)
{
	if (FIRSTLINE == nullptr)
	{
		FIRSTLINE = new LINE(lineNumber, line);
		return;
	}
	bool beforeFirst = true;
	if (usedLineNumber(lineNumber, beforeFirst) == true)
	{
		if (beforeFirst == true)
		{
			LINE *tmp = FIRSTLINE;
			FIRSTLINE = FIRSTLINE->ne;
			delete tmp;
		}
		else
			removeSourceLine(lineNumber);
	}
	LINE *ret = FIRSTLINE;
	if (beforeFirst == true)
	{
		ret = new LINE(lineNumber, line);
		ret->ne = FIRSTLINE;
		FIRSTLINE = ret;
		return;
	}
	else
	{
		while (ret->ne != nullptr && ret->ne->line_number < lineNumber)
			ret = ret->ne;
		if (ret->ne == nullptr)
			ret->ne = new LINE(lineNumber, line);
		else
		{
			LINE *tmp = new LINE(lineNumber, line);
			tmp->ne = ret->ne;
			ret->ne = tmp;
		}
		return;
	}
	return;
	// Replace this stub with your own code
}

bool Program::usedLineNumber(int lineNumber, bool &beforeFirst)
{
	if (lineNumber < FIRSTLINE->line_number)
	{
		beforeFirst = true;
		return false;
	}
	if (lineNumber == FIRSTLINE->line_number)
	{
		beforeFirst = true;
		return true;
	}
	beforeFirst = false;
	LINE *ret = FIRSTLINE;
	while (ret != nullptr && ret->line_number < lineNumber)
		ret = ret->ne;
	if (ret == nullptr)
		return false;
	if (ret->line_number == lineNumber)
		return true;
	else
		return false;
}

void Program::removeSourceLine(int lineNumber)
{
	LINE *ret = FIRSTLINE;
	LINE *pre = nullptr;
	while (ret != nullptr && ret->line_number != lineNumber)
	{
		pre = ret;
		ret = ret->ne;
	}
	if (ret == nullptr)
		return;
	if (pre == nullptr)
	{
		LINE *tmp = FIRSTLINE;
		FIRSTLINE = FIRSTLINE->ne;
		delete tmp;
		return;
	}
	else
	{
		LINE *tmp = ret;
		ret = ret->ne;
		pre->ne = ret;
		delete tmp;
		return;
	}
	// Replace this stub with your own code
}

string Program::getSourceLine(int lineNumber)
{
	if (FIRSTLINE == nullptr)
		return "";
	LINE *ret = FIRSTLINE;
	while (ret != nullptr && ret->line_number < lineNumber)
		ret = ret->ne;
	if (ret == nullptr)
		return "";
	if (ret->line_number != lineNumber)
		return "";
	return ret->str;
	// Replace this stub with your own code
}

void Program::setParsedStatement(int lineNumber, Statement *stmt)
{
	LINE *ret = FIRSTLINE;
	while (ret != nullptr && ret->line_number != lineNumber)
		ret = ret->ne;
	if (ret == nullptr)
	{
		//cout << "SYNTAX ERROR" << endl;
		return;
	}
	ret->state = stmt;
	//cout << "PROGRAM: " << stmt->tolineNumber << endl;
	return;
	// Replace this stub with your own code
}

Statement *Program::getParsedStatement(int lineNumber)
{
	LINE *ret = FIRSTLINE;
	while (ret != nullptr && ret->line_number != lineNumber)
		ret = ret->ne;
	if (ret == nullptr)
	{
		//cout << "SYNTAX ERROR" << endl;
		return nullptr;
	}
	if (ret->state == nullptr)
		return nullptr;
	else
		return ret->state;
	// Replace this stub with your own code
}

int Program::getFirstLineNumber()
{
	if (FIRSTLINE == nullptr)
		return -1;
	else
		return FIRSTLINE->line_number;
	// Replace this stub with your own code
}

int Program::getNextLineNumber(int lineNumber)
{
	LINE *ret = FIRSTLINE;
	while (ret != nullptr && ret->line_number <= lineNumber)
		ret = ret->ne;
	if (ret == nullptr)
		return -1;
	else
		return ret->line_number;
	// Replace this stub with your own code
}

void Program::RUN_(EvalState &state)
{
	//return;
	LINE *strline = FIRSTLINE;
	while (strline != nullptr)
	{
		//cout << strline->line_number << endl;
		Statement *tmpst = strline->state;
		if (tmpst->getTYPE() == remst)
		{
			strline = strline->ne;
			continue;
		}
		else if (tmpst->getTYPE() == endst)
		{
			break;
		}
		else if (tmpst->getTYPE() == letst || tmpst->getTYPE() == inputst || tmpst->getTYPE() == printst)
		{
			tmpst->execute(state);
			strline = strline->ne;
		}
		else //ifst / gotost
		{
			int num = tmpst->execute__(state);
			if (num == -2)
			{
				return;
				//cout << "num=-2" << endl;
			}
			else if (num == -1)
			{
				strline = strline->ne;
				//cout << "num = -1" << endl;
				continue;
			}
			else
			{
				strline = FIRSTLINE;
				while (strline != nullptr && strline->line_number != num)
					strline = strline->ne;
				if (strline == nullptr)
				{
					cout << "LINE NUMBER ERROR" << endl;
					return;
				}
				else
					continue;
			}
		}
	}
	return;
}
void Program::LIST_()
{
	LINE *strline = FIRSTLINE;
	while (strline != nullptr)
	{
		cout << strline->str << endl;
		strline = strline->ne;
	}
}
void Program::CLEAR_(Program &program, EvalState &state)
{
	program.clear();
	state.Clear();
	return;
}
void Program::HELP_()
{
	cout << "A Minimal BASIC Interpreter by LJX." << endl;
	return;
}
