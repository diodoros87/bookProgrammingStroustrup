#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;

inline void error(const string& errormessage) {
   throw runtime_error("error: " + errormessage);
}

inline void error(const string& s, const string& s2) {
	error(s + s2);
}

inline void error(const string& s, const char c) {
	cerr << "error: " << s << "'" << c << "'" << endl;
   throw runtime_error("");
}

string get_alphanum_string(char first) {
	string s = "";
	if (isalpha(first)) {   // string with only letter at start
		s += first;
		while (cin.get(first) && (isalpha(first) || isdigit(first)))
			s += first;
		cin.unget();
	}
	return s;
}

struct Token {
	char kind;
	double value;
	string name;
	
	Token(char ch) :kind(ch), value(0) { }
	Token(char ch, double val) :kind(ch), value(val) { }
	Token(char ch, string s) :kind(ch), name(s) { }
};

class Token_stream {
private:
	bool full;
	Token buffer;
	
	char  get_char();
	Token get_number();
	Token get_number_with_dot();
	Token get_other_tokens(char c);
public:
	Token_stream() :full(false), buffer(0) { }
	
	Token get();
	void unget(Token& t);
	void ignore(char, char);
	Token get_token_after_SPACE();
};

// The unget() member function puts its argument back into the Token_stream's buffer:
void Token_stream::unget(Token& t)
{
	if (full) 
		error("unget() into a full buffer");
	buffer = t;       // copy t to buffer
	full = true;      // buffer is now full
}

constexpr char declaration_key = '#';
constexpr char quit = 'q';
constexpr char print = ';';
constexpr char number = '8';
constexpr char name = 'a';
constexpr char SQRT = 'S';
constexpr char POWER = '^';
constexpr char SPACE = 's';
constexpr char NEW_LINE = '\n';

const string quit_name = "quit";
const string SQRT_NAME = "sqrt";

Token Token_stream::get_number() {	
	double val;
	cin >> val;
	if (cin)
		return Token(number, val);
	else
		error("Attempt to reading number has failed");
}

Token Token_stream::get_number_with_dot() {	
	char ch = get_char();
	cin.unget();
	if (false == isdigit(ch)) 
		error("Number started with dot must have digit as next char.");
	cin.putback('.');
	return get_number();
}

Token Token_stream::get_other_tokens(char first) {
	if (isspace(first)) 
		return Token(SPACE);
		
	string s = get_alphanum_string(first);
	if (s == quit_name) 
		return Token(quit);
	else if (s == SQRT_NAME) 
		return Token(SQRT);
	else if (s.size() > 0)
		return Token(name, s);

	error("Bad token: ", first);
}

Token Token_stream::get_token_after_SPACE() {
	Token t = get();
	while (t.kind == SPACE)
		t = get();
	return t;
}

char Token_stream::get_char() {
	char ch;
	cin.get(ch);
	if(!cin) {
		cout << "Exit \n";
		exit(0);
	}
	return ch;
}

Token Token_stream::get() {
	if (full) { 
		full = false; 
		return buffer; 
	}
	char ch = get_char();
	switch (ch) {
		case NEW_LINE:
		case '(': case ')': 
		case '+': case '-': 
		case '*': case '/': case '%':
		case '=':
		case '!':
		case print:
		case declaration_key:
		case POWER:
			return Token(ch);
		case '.':
			return get_number_with_dot();
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			cin.unget();
			return get_number();
		default:
			return get_other_tokens(ch);
	}
}

void Token_stream::ignore(char c1, char c2) {
	if (full && (c1 == buffer.kind || c2 == buffer.kind)) {
		full = false;
		return;
	}
	full = false;

	char ch;
	do {
		cin.get(ch);
	} while (cin && ch != c1 && ch != c2);

	return;
}

struct Variable {
	string name;
	double value;
	Variable(string n, double v) : name(n), value(v) { }
};

vector<Variable> names;

double get_value(string s)
{
	for (int i = 0; i < names.size(); ++i)
		if (names[i].name == s) 
			return names[i].value;
	error("get: undefined name ", s);
}

void set_value(string s, double d)
{
	for (int i = 0; i <= names.size(); ++i)
		if (names[i].name == s) {
			names[i].value = d;
			return;
		}
	error("set: undefined name ", s);
}

bool is_declared(string s)
{
	for (int i = 0; i < names.size(); ++i)
		if (names[i].name == s) 
			return true;
	return false;
}

Token_stream ts;

inline void error_and_unget(const string& errormessage, Token& t) {
	ts.unget(t);
   throw runtime_error("error: " + errormessage);
}

bool is_integer(double number) {
	int integer = number;
	if (integer == number)
		return true;
	else
		return false;
}

unsigned long factorial(double number) {
	if (0 > number)
		throw runtime_error("Precondition: can not calculate factorial for number < 0");
	if (false == is_integer(number))
		error("Precondition: calculate of factorial only for integer numbers");
	
	constexpr unsigned long U_LONG_MAX = numeric_limits<unsigned long>::max();
	unsigned long result = 1;
	for (short counter = 2; counter <= number; counter++) {
		if (result > U_LONG_MAX / counter) {
			cerr << "Calculating factorial of " << number << " = " << number << "! :\n";
			cerr << "While calculating factorial of " << counter << " = " << counter << "! :\n";
			cerr << 	" result = " << result << " U_LONG_MAX = " << U_LONG_MAX << " \n";
			cerr << "result > U_LONG_MAX / counter that is " << result << " > " << U_LONG_MAX / counter << "\n";
			cerr << "result * counter = " << result * counter << "\n";
			throw overflow_error("unsigned long overflow error");
		}
		result = result * counter;
	}
	
	if (0 >= result)
		throw runtime_error("Final condition: result of factorial can not be number <= 0");
		
	return result;
}

double primary();

double square_root() {
	Token t = ts.get_token_after_SPACE();
	if ('(' != t.kind) 
		error_and_unget("number of square root must be in brackets", t);
	ts.unget(t);
	double x = primary();
	if (x < 0)
		error("Real solution for square root for number < 0 does not exist");
	return sqrt(x);
}

double power(double base, int exponent) {
	if (0 == base) {
		if (0 != exponent)
			return 0;
		error("0 to power of 0 is indeterminate form");
	}
	double result = 1;
	unsigned int last = exponent > 0 ? exponent : -exponent;
	unsigned int counter = 1;
	while(counter++ <= last)
		result *= base;
	if (exponent < 0)
		result = 1 / result;
	return result;
}

double power(double base) {
	Token t = ts.get_token_after_SPACE();
	if ('(' != t.kind) 
		error_and_unget("in power calculation exponent must be in brackets", t);
	ts.unget(t);
	double exponent = primary();  // brackets are in primary()
	if (false == is_integer(exponent))
		error("calculate of power only for integer exponent");
	double result = power(base, exponent);
	return result;
}

bool curly_braces = false;
bool square_braces = false;
bool operation = false;

double expression();
/*
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
		error("Unrecognized opening bracket ", bracket_kind);
		
	double d = expression();
   Token t = ts.get();
   if (t.kind != last_bracket)
		error("closed bracket expected: ", last_bracket);
		 
   return d;
}
*/
double primary() {
	double result;
	Token t = ts.get_token_after_SPACE();
	switch (t.kind) {
	case '(':
		result = expression();
		t = ts.get();
		if (t.kind != ')')
			error_and_unget("')' expected", t);
		operation = false;
		break;
	case '+':
	case '-':
		if (true == operation)
			error("Next token after operator can not be + or -");
		operation = true;
		result = '+' == t.kind ? primary() : -primary();
		break;
	case NEW_LINE:   // to allow many lines calculations without ignoring new line tokens in cin and Token_stream
		result = primary();
		break;
	case number:
		result = t.value;
		operation = false;
		break;
	case name:
		result = get_value(t.name);
		operation = false;
		break;
	default:
		error("unrecognized primary: ", t.kind);
	}
	
	return result;
}

double factor();

double before_primary(Token& t, bool& minus_number) {
	switch (t.kind) {
		case SQRT:
			return square_root();
		case '-':    // to allow minus '-' as first char in expression with factorial
			if (operation)
				error("Next token after operator can not be + or -");
			minus_number = true;    // -4! == -24    (-4)! error
			operation = true;
			return factor();
		default:
			ts.unget(t);
			return primary();
	}
}

double after_primary(double x, Token& t) {
	double result;
	switch (t.kind) {
		case POWER:
			if (operation)
				error("Next token after operator can not be ", POWER);
			result = power(x);
			break;
		case '!':    // to allow minus '-' as first char in expression with factorial
			if (operation)
				error("Next token after operator can not be '!'");
			result = factorial(x);  // -4! == -24    (-4)! error
			break;
		default:
			ts.unget(t);
			result = x;
	}
	return result;
}

double factor() {
	bool minus_number = false;
	Token t = ts.get_token_after_SPACE();
	double result = before_primary(t, minus_number);
	t = ts.get_token_after_SPACE();
	result = after_primary(result, t);
	if (minus_number)
		result = -result;
	return result;
}

double term() {
	double left = factor();
	operation = true;
	while (true) {
		Token t = ts.get_token_after_SPACE();
		switch (t.kind) {
		case '*':
			left *= factor();
			break;
		case '/':
		case '%':
			{	
				double d = factor();
				if (d == 0) 
					error("divide by zero");
				if (t.kind == '/')
					left /= d;
				else
					left = fmod(left, d);
				break;
			}
		default:
			ts.unget(t);
			return left;
		}
	}
}

double expression() {
	operation = false;
	double left = term();
	operation = true;
	while (true) {
		Token t = ts.get_token_after_SPACE();
		switch (t.kind) {
		case '+':
			left += term();
			break;
		case '-':
			left -= term();
			break;
		default:
			ts.unget(t);
			return left;
		}
	}
}

double declaration() {
	Token t = ts.get();
	if (t.kind != name) 
		error("name expected in declaration");
	string name = t.name;
	if (is_declared(name)) 
		error(name, " declared twice");
	Token t2 = ts.get();
	if (t2.kind != '=') 
		error("= missing in declaration of ", name);
	double d = expression();
	names.push_back(Variable(name, d));
	return d;
}

double statement() {
	Token t = ts.get();
	switch (t.kind) {
	case declaration_key:
		return declaration();
	default:
		ts.unget(t);
		return expression();
	}
}

void clean_up_mess() {
	ts.ignore(print, NEW_LINE);
}

bool is_running() {
	Token t = ts.get();
	while (t.kind == print || t.kind == NEW_LINE || t.kind == SPACE)
		t = ts.get();
	if (t.kind == quit) 
		return false;
	else 
		ts.unget(t);
	return true;
}

void calculate() {
	const string prompt = "> ";
	const string result = "= ";
	double value = exp(1);   // Euler number	
	do {
		try {
			cout << prompt;
			if (false == is_running())
				return;
			value = statement();             // separation in call of statement() and
			cout << result << value << endl; // call of cout to do not allow display string of result 
			                                 // before end of writing many lines calculations
		} 
		catch (runtime_error& e) {
			cerr << e.what() << endl;
			clean_up_mess();
		}
	} while (true);
}

void enter_key(char key) {
	cout << "Enter \'" << key << "\' to continue ";
	char c;
	while (cin.get(c) && c != key)
		continue;
}

int main()
try {
	names.push_back(Variable("k", 1000));
	calculate();
	return 0;
}
catch (exception& e) {
	cerr << "exception: " << e.what() << endl;
	enter_key(print);
	return 1;
}
catch (...) {
	cerr << "unrecognized exception\n";
	enter_key(print);
	return 2;
}
