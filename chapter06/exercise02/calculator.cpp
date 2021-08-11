
//
// This is example code from Chapter 6.7 "Trying the second version" of
// "Software - Principles and Practice using C++" by Bjarne Stroustrup
//

/*
    This file is known as calculator02buggy.cpp

    I have inserted 5 errors that should cause this not to compile
    I have inserted 3 logic errors that should cause the program to give wrong results

    First try to find an remove the bugs without looking in the book.
    If that gets tedious, compare the code to that in the book (or posted source code)

    Happy hunting!

*/

//#include "../std_lib_facilities.h"

#include <iostream>

using namespace std;

constexpr char QUIT = 'x';
constexpr char PRINT = '=';

inline void error(const string& errormessage)
{
	cerr << errormessage << endl;
   throw runtime_error(errormessage);
}

inline void error(const string& errormessage, char c)
{
	cerr << errormessage << c << endl;
   throw runtime_error("");
}


//------------------------------------------------------------------------------

class Token {
public:
   char kind;        // what kind of token
   double value;     // for numbers: a value 
   Token(char ch)    // make a Token from a char
      :kind(ch), value(0) { }
   Token(char ch, double val)     // make a Token from a char and a double
      :kind(ch), value(val) { }
};

//------------------------------------------------------------------------------

class Token_stream {
public:
   Token_stream();   // make a Token_stream that reads from cin
   Token get();      // get a Token (get() is defined elsewhere)
   void putback(Token t);    // put a Token back
private:
   bool full;        // is there a Token in the buffer?
   Token buffer;     // here is where we keep a Token put back using putback()
};

//------------------------------------------------------------------------------

// The constructor just sets full to indicate that the buffer is empty:
Token_stream::Token_stream()
   :full(false), buffer(0)    // no Token in buffer
{
}

//------------------------------------------------------------------------------

// The putback() member function puts its argument back into the Token_stream's buffer:
void Token_stream::putback(Token t)
{
	if (full) 
		error("putback() into a full buffer");
	buffer = t;       // copy t to buffer
	full = true;      // buffer is now full
}

//------------------------------------------------------------------------------

// The get() member function get from the Token_stream's buffer or cin:
Token Token_stream::get()
{
	if (full) {       // do we already have a Token ready?
	  // remove token from buffer
	  full = false;
	  return buffer;
	}

	char ch;
	cin >> ch;    // note that >> skips whitespace (space, newline, tab, etc.)
	
	if(!cin)
		exit(0);

	switch (ch) {
		case PRINT:    // for "print"
		case QUIT:    // for "quit"
		case '(': case ')': case '{': case '}': 
		case '+': case '-': case '*': case '/':
		  return Token(ch);        // let each character represent itself
		case '.':
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '9': case '8':
		{
		  cin.putback(ch);         // put digit back into the input stream
		  double val;
		  cin >> val;              // read a floating-point number
		  return Token('8', val);   // let '8' represent "a number"
		}
		default:
			error("Bad token");
	}
}

//------------------------------------------------------------------------------

Token_stream ts;        // provides get() and putback() 

//------------------------------------------------------------------------------

double expression(bool curly_braces);    // declaration so that brackets_pair() can call expression(bool)

//------------------------------------------------------------------------------

double brackets_pair(char bracket_kind) {
	char last_bracket = 0;
	bool curly_braces = false;
	if (bracket_kind == '{') {
		last_bracket = '}';
		curly_braces = true;
	}
	else if (bracket_kind == '(')
		last_bracket = ')';
	else
		error("Unrecognized opening bracket");
		
	double d = expression(curly_braces);
   Token t = ts.get();
   if (t.kind != last_bracket)
		 error("closed bracket expected: ", last_bracket);
		 
   return d;
}

// deal with numbers and parentheses
double primary(bool curly_braces)
{
	Token t = ts.get();
	switch (t.kind) {
		case '{':    // handle '{' expression '}'
			if (false == curly_braces)
				error("Before close brace ), curly brace { is not accepted");
		case '(':    // handle '(' expression ')'
		{
			double d = brackets_pair(t.kind);
			return d;
		}
		case QUIT:
			exit(0);
		case '8':            // we use '8' to represent a number
		  return t.value;  // return the number's value
		default:
		  error("primary expected");
	}
}

//------------------------------------------------------------------------------

// deal with *, /, and %
double term(bool curly_braces)
{
	double left = primary(curly_braces);
	Token t = ts.get();        // get the next token from token stream

	while (true) {
		switch (t.kind) {
			case '*':
				left *= primary(curly_braces);
				t = ts.get();
				break;
			case '/':
			{
				double d = primary(curly_braces);
				if (d == 0) error("divide by zero");
				left /= d;
				t = ts.get();
				break;
			}
			default:
				ts.putback(t);     // put t back into the token stream
				return left;
		}
	}
}

//------------------------------------------------------------------------------

// deal with + and -
double expression(bool curly_braces)
{
	double left = term(curly_braces);      // read and evaluate a Term
	Token t = ts.get();        // get the next token from token stream

	while (true) {
		switch (t.kind) {
			case '+':
				left += term(curly_braces);    // evaluate Term and add
				t = ts.get();
				break;
			case '-':
				left -= term(curly_braces);    // evaluate Term and subtract
				t = ts.get();
				break;
			default:
				ts.putback(t);     // put t back into the token stream
				return left;       // finally: no more + or -: return the answer
		}
	}
}

//------------------------------------------------------------------------------

void print_info() {
	cout << "Welcome in simple calculator.\n Use floating-point numbers." << endl;
	cout << "Supported operators: \n";
	cout << "+ - * /\n";
	cout << "Supported set of brackets: \n";
	cout << "{} () {()}\n";
	cout << "To print result press " << PRINT << endl;
	cout << "To quit press "<< QUIT << endl;
}

int main()
try
{
	print_info();

	bool curly_braces = true;
	double val = 0;
	while (cin) {
	  Token t = ts.get();

	   if (t.kind == QUIT) 
			break; // for quit
	   if (t.kind == PRINT)        // for "print now"
			cout << " = " << val << '\n';
	   else
			ts.putback(t);
	   val = expression(curly_braces);
	}
}
catch (exception& e) {
    cerr << "error: " << e.what() << '\n';
    return 1;
}
catch (...) {
    cerr << "Oops: unknown exception!\n";
    return 2;
}

//------------------------------------------------------------------------------
