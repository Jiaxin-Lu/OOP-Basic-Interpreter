/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
 * excellent model for the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/strlib.h"

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */
enum StatementTYPE
{
	errorst = -1,
	remst,
	inputst,
	letst,
	printst,
	endst,
	ifst,
	gotost
};
class Statement
{

  public:
	/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

	Statement();

	/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

	virtual ~Statement();

	/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

	virtual void execute(EvalState &state){};
	virtual void execute_(){};
	virtual int execute__(EvalState &state) { return 0; }
	virtual StatementTYPE getTYPE() { return errorst; }
	virtual bool CheckError() { return false; }
	virtual void execute_direct(EvalState &state){};
	bool CheckKeyWord(string str);
	bool CheckVar(string str, EvalState &state);
};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */
class REMST : public Statement
{
  public:
	REMST()
	{
		/*Empty*/
	}
	virtual void execute_();
	virtual StatementTYPE getTYPE();
};

class LETST : public Statement
{
  public:
	LETST(string str) : line(str), var(""), expstr(""), exp(nullptr){};
	virtual void execute(EvalState &state);
	virtual StatementTYPE getTYPE();
	virtual bool CheckError();
	virtual void execute_direct(EvalState &state);

  private:
	string line;
	string var;
	string expstr;
	Expression *exp;
};

class PRINTST : public Statement
{
  public:
	PRINTST(string str) : line(str), expstr(""), exp(nullptr){};
	virtual void execute(EvalState &state);
	virtual StatementTYPE getTYPE();
	virtual bool CheckError();

  private:
	string line;
	string expstr;
	Expression *exp;
};

class INPUTST : public Statement
{
  public:
	INPUTST(string str) : line(str), var(""){};
	virtual void execute(EvalState &state);
	virtual StatementTYPE getTYPE();
	virtual bool CheckError();
	int inputInteger(string prompt);
	//bool isNumber(string str);
  private:
	string line;
	string var;
};

class ENDST : public Statement
{
  public:
	ENDST(string str) : line(str){};
	virtual void execute_();
	virtual StatementTYPE getTYPE();
	virtual bool CheckError();

  private:
	string line;
};

class GOTOST : public Statement
{
  public:
	GOTOST(string str) : line(str), tolineNumber(-1){};
	virtual int execute__(EvalState &state);
	virtual StatementTYPE getTYPE();
	virtual bool CheckError();

  private:
	string line;
	int tolineNumber;
};

class IFST : public Statement
{
  public:
	IFST(string str) : line(str), exp1str(""), exp2str(""), exp1(nullptr), exp2(nullptr), tolineNumber(-1){};
	virtual int execute__(EvalState &state);
	virtual StatementTYPE getTYPE();
	virtual bool CheckError();

  private:
	string line;
	string exp1str;
	string exp2str;
	string op;
	Expression *exp1;
	Expression *exp2;
	int tolineNumber;
};
#endif
