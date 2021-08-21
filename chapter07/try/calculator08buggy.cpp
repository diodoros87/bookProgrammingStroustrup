
/*
	calculator08buggy.cpp

	Helpful comments removed.

	We have inserted 3 bugs that the compiler will catch and 3 that it won't.
*/

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

inline void error(const string& errormessage)
{
	cerr << errormessage << endl;
   throw runtime_error("");
}

inline void error(const string& s, const string& s2)
{
	error(s + s2);
}

inline void error(const string& s, const char c)
{
	cerr << s << c << endl;
   throw runtime_error("");
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
public:
	Token_stream() :full(false), buffer(0) { }
	
	Token get();
	void unget(Token t);
	void ignore(char);
};

// The unget() member function puts its argument back into the Token_stream's buffer:
void Token_stream::unget(Token t)
{
	if (full) 
		error("unget() into a full buffer");
	buffer = t;       // copy t to buffer
	full = true;      // buffer is now full
}

constexpr char let = 'L';
constexpr char quit = 'Q';
constexpr char print = ';';
constexpr char number = '8';
constexpr char name = 'a';

const string declkey = "let";

Token Token_stream::get()
{
	if (full) { 
		full = false; 
		return buffer; 
	}
	char ch;
	cin >> ch;
	if(!cin)
		exit(0);
	switch (ch) {
		case '(': case ')': 
		case '+': case '-': 
		case '*': case '/': case '%':
		case '=':
		case print:
		case quit:
			return Token(ch);
		case '.':
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			{	
				cin.unget();
				double val;
				cin >> val;
				if(!cin) 
					error("Number started with dot must have digit as next char");
				return Token(number, val);
			}
		default:
			if (isalpha(ch)) {
				string s;
				s += ch;
				while (cin.get(ch) && (isalpha(ch) || isdigit(ch)))
					s += ch;
				cin.unget();
				if (s == declkey) 
					return Token(let);
				else
					return Token(name, s);
			}
			error("Bad token", ch);
	}
}

void Token_stream::ignore(char c)
{
	if (full && c == buffer.kind) {
		full = false;
		return;
	}
	full = false;

	char ch;
	do {
		cin >> ch;
	} while (cin && ch != c);
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

double expression();

double primary()
{
	Token t = ts.get();
	switch (t.kind) {
	case '(':
		{	
			double d = expression();
			t = ts.get();
			if (t.kind != ')') 
				error("'(' expected");
			return d;
		}
	case '-':
		return -primary();
	case '+':
		return primary();
	case number:
		return t.value;
	case name:
		return get_value(t.name);
	default:
		error("primary expected");
	}
}

double term()
{
	double left = primary();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		case '*':
			left *= primary();
			break;
		case '/':
			{	
				double d = primary();
				if (d == 0) 
					error("divide by zero");
				left /= d;
				break;
			}
		case '%':
			{	
				double d = primary();
				if (d == 0) 
					error("divide by zero");
				left = fmod(left, d);
				break;
			}
		default:
			ts.unget(t);
			return left;
		}
	}
}

double expression()
{
	double left = term();
	while (true) {
		Token t = ts.get();
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

double declaration()
{
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

double statement()
{
	Token t = ts.get();
	switch (t.kind) {
	case let:
		return declaration();
	default:
		ts.unget(t);
		return expression();
	}
}

void clean_up_mess()
{
	ts.ignore(print);
}

const string prompt = "> ";
const string result = "= ";

void calculate()
{
	while (cin) 
	try {
		cout << prompt;
		Token t = ts.get();
		while (t.kind == print) 
			t = ts.get();
		if (t.kind == quit) 
			return;
		ts.unget(t);
		cout << result << statement() << endl;
	}
	catch (runtime_error& e) {
		cerr << e.what() << endl;
		clean_up_mess();
	}
}

void enter_key(char key) {
	cout << "Enter \'" << key << "\' to continue ";
	char c;
	while (cin >> c && c != key)
		continue;
}

int main()
try {
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
