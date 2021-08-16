#include <iostream>
#include <limits>

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
   throw runtime_error(errormessage);
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
		case '+': case '-': case '*': case '/': case '!': case '%':
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
			error("Bad token: ", ch);
	}
}

//------------------------------------------------------------------------------

Token_stream ts;        // provides get() and putback() 

//------------------------------------------------------------------------------

double expression(bool curly_braces);    // declaration so that brackets_pair() can call expression(bool)

//------------------------------------------------------------------------------

bool is_integer(double number) {
	int integer = number;
	if (integer == number)
		return true;
	else
		return false;
}

long factorial(double number) {
	if (0 > number)
		error("Can not calculate factorial for number < 0");
	if (false == is_integer(number))
		error("Can not calculate factorial for non-integer number");
	
	long result = 1;
	long next_result;
	for (int counter = 2; counter <= (int)number; counter++) {
		next_result = result * counter;
		if (next_result <= result) {
			cerr << "Arithmetic overflow (next_result <= result):" << endl;
			cerr << "next_result = " << next_result << endl;
			cerr << "result      = " << result << endl;
			cerr << " numeric_limits<int>::max() = " << numeric_limits<int>::max() << endl;
			cerr << " numeric_limits<long>::max() = " << numeric_limits<long>::max() << endl;
			throw runtime_error("Arithmetic overflow error");
		}
		result = next_result;
	}
		
	return result;
}

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

double calculate_result(double x) {
	double result = x;
	Token t = ts.get();
	if (t.kind == '!')
		result = factorial(x);
	else
		ts.putback(t);     // put t back into the token stream
	return result;
}

// deal with numbers and parentheses
double primary(bool curly_braces, bool operation)
{
	double result = 0;
	Token t = ts.get();
	switch (t.kind) {
		case QUIT:
			exit(0);
		case '{':    // handle '{' expression '}'
			if (false == curly_braces)
				error("Before close brace ), curly brace { is not accepted");
		case '(':    // handle '(' expression ')'
			result = brackets_pair(t.kind);
			break;
		case '8':           // we use '8' to represent a number
		   result = t.value;
			break;
		case '+':
		case '-':
			if (true == operation)
				error("Next token after operator can not be + or -");
			result = primary(curly_braces, true);
			break;
		default:
		   error("primary expected");
	}
	
	result = calculate_result(result);
	if ('-' == t.kind)
		result = -result;
	return result;
}

//------------------------------------------------------------------------------

// deal with *, /, and %
double term(bool curly_braces, bool operation)
{
	double left = primary(curly_braces, operation);
	operation = true;
	Token t = ts.get();        // get the next token from token stream
	
	while (true) {
		switch (t.kind) {
			case '*':
				left *= primary(curly_braces, operation);
				t = ts.get();
				break;
			case '/':
			case '%':
			{
				double d = primary(curly_braces, operation);
				if (d == 0) 
					error("divide by zero");
					
				if ('/' == t.kind)
					left /= d;
				else if (false == is_integer(left) || false == is_integer(d))
					error("In modulo division all elements must be integers");
				else
					left = int(left) % int(d);

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
	bool operation = false;
	double left = term(curly_braces, operation);      // read and evaluate a Term
	operation = true;
	Token t = ts.get();        // get the next token from token stream

	while (true) {
		switch (t.kind) {
			case '+':
				left += term(curly_braces, operation);    // evaluate Term and add
				t = ts.get();
				break;
			case '-':
				left -= term(curly_braces, operation);    // evaluate Term and subtract
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
	cout << "+ - * / %\n";
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
