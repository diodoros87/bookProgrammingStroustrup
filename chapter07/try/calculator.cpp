#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

inline void error(const string& errormessage)
{
   throw runtime_error("error: " + errormessage);
}

inline void error(const string& s, const string& s2)
{
	error(s + s2);
}

inline void error(const string& s, const char c)
{
	cerr << "error: " << s << "\'" << c << "\'" << endl;
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
	
	char  get_char();
	Token get_number();
	Token check_dot();
	Token check_other_tokens(char c);
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

constexpr char declaration_key = '#';
constexpr char quit = 'q';
constexpr char print = ';';
constexpr char number = '8';
constexpr char name = 'a';

constexpr char NEW_LINE = '\n';

const string quit_name = "quit";

Token Token_stream::get_number() {	
	double val;
	cin >> val;
	if (cin)
		return Token(number, val);
	else
		throw runtime_error("Attempt to reading number has failed");
}

Token Token_stream::check_dot() {	
	cin.unget();
	try {
		return get_number();
	}
	catch (runtime_error& e) {
		cerr << "exception: " << e.what() << endl;
		error("Number started with dot must have digit as next char");
	}
}

Token Token_stream::check_other_tokens(char ch) {
	if (isalpha(ch)) {
		string s;
		s += ch;
		while (cin.get(ch) && (isalpha(ch) || isdigit(ch)))
			s += ch;
		cin.unget();
		if (s == quit_name) 
			return Token(quit);
		else
			return Token(name, s);
	}
	error("Bad token: ", ch);
}

char Token_stream::get_char() {
	char ch;
	do {
		cin.get(ch);
	} while (cin && isspace(ch) && ch != NEW_LINE);
	if(!cin)
		exit(0);
	return ch;
}

Token Token_stream::get()
{
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
		case print:
		case declaration_key:
			return Token(ch);
		case '.':
			return check_dot();
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			cin.unget();
			return get_number();
		default:
			return check_other_tokens(ch);
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
		cin.get(ch);
	} while (cin && ch != c && ch != NEW_LINE);

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
	case NEW_LINE:
		return primary();
	case number:
		return t.value;
	case name:
		return get_value(t.name);
	default:
		error("primary has not: ", t.kind);
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
		case NEW_LINE:
			;
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
		case NEW_LINE:
			;
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
	case declaration_key:
		return declaration();
	default:
		ts.unget(t);
		return expression();
	}
}

void clean_up_mess() {
	ts.ignore(print);
}

bool is_running() {
	Token t = ts.get();
	while (t.kind == print || t.kind == NEW_LINE) 
		t = ts.get();
	ts.unget(t);
	if (t.kind == quit) 
		return false;
	else
		return true;
}

const string prompt = "> ";
const string result = "= ";

void calculate() {
	double value = exp(1);   // Euler number	
	cout << prompt;
	do {
		try {
			if (false == is_running())
				return;
			value = statement();
			Token t = ts.get();
			if (t.kind == print) {
				cout << result << value << endl;
				cout << prompt;
			}
			else
				ts.unget(t);
		} 
		catch (runtime_error& e) {
			cerr << e.what() << endl;
			clean_up_mess();
			cout << prompt;
		}
	} while (true);
}

void enter_key(char key) {
	cout << "Enter \'" << key << "\' to continue ";
	char c;
	while (cin >> c && c != key)
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
