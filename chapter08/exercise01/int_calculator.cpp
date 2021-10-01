#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <climits>

using namespace std;

// Errors
//--------------------------------------------
inline void error(const string& errormessage) {
   throw runtime_error("!!!!! Error: " + errormessage);
}

inline void error(const string& s, const string& s2) {
	error(s + s2);
}

inline void error(const string& s, const char c) {
	cerr << "!!!!! Error: " << s << "'" << c << "'" << endl;
   throw runtime_error("");
}

inline void error(const string& s, int i) {
	cerr << "!!!!! Error: " << s << i <<  endl;
   throw runtime_error("");
}

bool is_in_longlong_range(double val) {
	if (val >= LLONG_MIN && val <= LLONG_MAX)
		return true;
	else
		return false;
}

bool is_integer(double x) {
	int integer = x;
	if (integer != x) {
		if (is_in_longlong_range(x) && 0 == fmod(x, 1))
			cerr << "Parameter long long x = " << (long long)x << endl;
		else
			cerr << "Parameter double x = " << x << endl;
		cerr << "Int of x = " << integer << endl;
		return false;
		//error("Parameter is NOT int type");
	}
	return true;
}

void check_integer_range(double val) {
	if (val > INT_MAX)
		error("Max number for int is ", INT_MAX);
	if (val < INT_MIN)
		error("Min number for int is ", INT_MIN);
}

// Token char kinds
//--------------------------------------------
constexpr char declaration_key = '#';
constexpr char quit = 'q';
constexpr char print = ';';   
constexpr char number = '8';
constexpr char NAME = 'a';
constexpr char HELP = 'h';
constexpr char SQRT = 'S';
constexpr char POWER = '^';
constexpr char SPACE = 's';
constexpr char NEW_LINE = '\n';
constexpr char CONST = 'C';
constexpr char VARS_VIEW = 'v';

// Token program keywords
//--------------------------------------------
constexpr char* quit_name = "quit";
constexpr char* POWER_NAME = "pow";
constexpr char* CONST_NAME = "const";
constexpr char* VARS_VIEW_NAME = "vars_view";
constexpr char* SQRT_NAME = "sqrt";
constexpr char* HELP_NAME = "help";

// Token program keywords can not be used as variable name
const vector<char> NOT_VARIABLES_TOKENS = { quit, SQRT, HELP, print, POWER, declaration_key,
														CONST, VARS_VIEW };
														
bool equals_case_insensitive(string s1, string s2) {
	const unsigned int SIZE = s1.size();
	if (SIZE != s2.size())
		return false;
		
	for (unsigned int i = 0; i < SIZE; i++) {
		char c1 = tolower(s1[i]);
		char c2 = tolower(s2[i]);
		if (c1 != c2)
			return false;
	}
	
	return true;
}

bool exist(char c, const vector<char>& vec) {
	for (char a : vec) 
		if (a == c) 
			return true;		
	return false;
}

const vector<string> NOT_VARIABLES_NAMES = { quit_name, SQRT_NAME, CONST_NAME, VARS_VIEW_NAME,
											HELP_NAME, POWER_NAME };

bool exist(string s, const vector<string>& vec) {
	for (string a : vec) 
		if (a == s) 
			return true;		
	return false;
}

// Token struct
//--------------------------------------------
struct Token {    // Token has only non-negative numbers, because every token of '-'(minus) char is get separately from number
	char kind;     
	unsigned int value;    // absolute value of labs(INT_MIN) == labs(INT_MAX + 1)
	string name;
	
	Token(char ch) :kind(ch), value(0) { }
	Token(char ch, double val);
	//Token(char ch, double val) :kind(ch), value(val) { }
	Token(char ch, string s) :kind(ch), name(s) { }
};

// Token has only non-negative numbers, because every token of '-'(minus) char is get separately from number
Token::Token(char ch, double val) { 
	if (val != -(long long)(INT_MIN) && false == is_integer(val))  // absolute value of labs(INT_MIN) == labs(INT_MAX + 1)
		error("Precondition: Token value must be int type");
	kind = ch;
	value = val;
}

string get_name(const Token& t) {
	char kind = t.kind;
	switch (kind) {
		case quit:
			return quit_name;
		case number:
			return "number";
		case NAME:
			return t.name;
		case SQRT:
			return SQRT_NAME;
		case HELP:
			return HELP_NAME;
		case print:
			return string(1, print);
		case POWER:
			return POWER_NAME;
		case declaration_key:
			return string(1, declaration_key);
		case CONST:
			return CONST_NAME;
		case VARS_VIEW:
			return VARS_VIEW_NAME;
		default:
			return "";
	}
}

const string MANUAL_STRING = string("MANUAL for simple calculator. \n\
Using only int type numbers and variables for calculation.\n\
Max number for int is ") + to_string(INT_MAX) +
string(". Min number for int is ") + to_string(INT_MIN) +
"\nIn below informations 'x', 'y', 'z' are treated as number or variable \n\
Signed numbers or variables -x (-x) +x (+x) are allowed \
but --x x++ -+x are unacceptable\n\
To their accept necessary is separation by brackets -(-x) +(-y) \
but every sign must be separated by number or bracket \n\
Operators can not follow each other - between operators must be bracket or variable or number:\n\
\+x*y-z*(-x/t) is OK, but x/-z is unacceptable \n\
Supported set of brackets: \n\
{} [] {[]} () [()] {()} {[()]} \n\
Each kind of brackets may be inside the same kind of bracket: (()) [[]] {{{}}} \n\
{} can not be be inside () and []  \n\
[] can not be be inside ()  \n\
Supported operations: \n\
1. addition x+y \n\
2. subtraction x-y \n\
3. multiplication x*y \n\
4. division x/y \n\
5. modulo division x%y \n\
6. square root sqrt(x) - number of sqrt must be in any supported bracket kind \n\
7. power(exponentiation) " + POWER_NAME + "(x, y) (x to power of y - y is exponent) \n\
number of exponent must be in any supported bracket kind \n\
8. factorial x! \n\
To separate many calculations press " + string(1, print) + " or whitespace or new line \n\
To do operation in many lines enter operator directly before press new line for example 2+3+ENTER: \n\
In new line operation will be continue \n\
To define new variable enter " + string(1, declaration_key) + " whitespace, variable name = value(number or other variable)\n\
Variable name must started with letter or _ and may contain letters, digits and _ \n\
Can not declare identical variable name \n\
To define constant 'variable' enter: " + string(1, declaration_key) + " " + CONST_NAME + " name_of_variable \n\
To display already defined variables enter " + VARS_VIEW_NAME + "\n\
To quit enter " + quit_name + "\n\
To display this manual enter " + HELP_NAME + " case insensitive \n";

// Token_stream class
//--------------------------------------------
class Token_stream {
private:
	bool full;
	Token buffer;
   istream& input_stream;
	
	Token get_number();
	Token get_single_char_token(char first);
	bool  is_help_token(string s);
	Token get_alphanum_token(char first);
	Token get_other_token(char c);
public:
   Token_stream(istream& input_stream) 
            :full(false), buffer(0), input_stream(input_stream) { 
               
   }
	
	char get_char();
	Token get();
	void unget(Token& t);
	void ignore(char, char);
	Token get_token_after_SPACE();
	void check_name(Token& t);
	bool is_constant_token(Token& t);
	string get_name_string(char first);
	string get_name_string();
};

char Token_stream::get_char() {
	char ch;
	input_stream.get(ch);
	if(! input_stream) {
		cout << "Exit \n";
		exit(0);
	}
	return ch;
}

string Token_stream::get_name_string() {
	string s = "";
	char c;
	while (input_stream.get(c) && ('_' == c || isalpha(c) || isdigit(c)))
		s += c;
			
	input_stream.unget();   // to save non-alphanumerical character in input stream
	return s;
}

string Token_stream::get_name_string(char first) {
	string s = "";
	if (isalpha(first) || '_' == first)   // string with only letter at start
		s = first + get_name_string();     // get alphanumerical string
	return s;
}

void Token_stream::unget(Token& t) {
	if (full) 
		error("unget() into a full buffer");
	buffer = t;       // copy t to buffer
	full = true;      // buffer is now full
}

// function get only non-negative numbers, because every token of '-' char is get separately from number
Token Token_stream::get_number() {	 
	double val;
	input_stream >> val;
	if (!input_stream)
		error("Attempt to reading number has failed");
	if (val != -(long long)(INT_MIN) && false == is_integer(val))  // absolute value of labs(INT_MIN) == labs(INT_MAX + 1)
		error("input value is not int type ");
	
	return Token(number, val);
}

Token EMPTY_TOKEN = Token(0);  // to signal that get of token is unsuccessfull

Token Token_stream::get_single_char_token(char first) {
	if (isspace(first)) 
		return Token(SPACE);
		
	if (first != declaration_key)
		return EMPTY_TOKEN; // not single token
		
	Token result = EMPTY_TOKEN;
	char next = get_char();   // get next char after 'first' char
	input_stream.unget();   // to save 'next' character in input stream
	if (first == declaration_key) {
		if (next != '\n' && isspace(next))
			result = Token(declaration_key);
		else
			error("After declaration key must be whitespace except new line");
	} 
	return result;
}

bool Token_stream::is_help_token(string s) {
	if (false == equals_case_insensitive(s, HELP_NAME))
		return false;
	
	char next = get_char();   // get next char after string s
	input_stream.unget();   // to save 'next' character in input stream	
	if (next == print || isspace(next))
		return true;
	else if (! isalnum(next))
		error("After help name must be whitespace or " + string(1, print) + 
						" or alphanumerical key or " + declaration_key);
}

Token Token_stream::get_alphanum_token(char first) {
	string s = get_name_string(first);
	if (s == quit_name) 
		return Token(quit);
	else if (s == POWER_NAME) 
		return Token(POWER);
	else if (s == SQRT_NAME) 
		return Token(SQRT);
	else if (s == CONST_NAME) 
		return Token(CONST);
	else if (s == VARS_VIEW_NAME) 
		return Token(VARS_VIEW);
	else if (is_help_token(s))
		return Token(HELP);
	else if (s.size() > 0) 
		return Token(NAME, s);
		
	return EMPTY_TOKEN;
}

void Token_stream::check_name(Token& t) {
	if (exist(t.kind, NOT_VARIABLES_TOKENS)) {
		string keyword_name = get_name(t);
		error(keyword_name + " is keyword and can not be used as variable");
	}
	if (exist(t.name, NOT_VARIABLES_NAMES)) 
		error(t.name + " is keyword and can not be used as variable");
	if (t.kind != NAME) { 
		unget(t);
		error("variable name expected in declaration",
			" name must started with letter or _ and may contain letters, digits and _ ");
	}
}

bool Token_stream::is_constant_token(Token& t) {
	bool result = false;
	if (t.kind == CONST) { 
		result = true;
		t = get_token_after_SPACE();  // if constant token then get next token (should be variable name)
		if (t.kind == '=' || t.kind == NEW_LINE) {
			unget(t);
			error(string(CONST_NAME) + " is keyword and can not be used as variable. \n",
				"After this keyword must be one new declared name, = as assignment operator, value");
		}
	}
	return result;
}

Token Token_stream::get_other_token(char first) {
	Token result = get_single_char_token(first);
	if (result.kind == EMPTY_TOKEN.kind)	
		result = get_alphanum_token(first);
	if (result.kind != EMPTY_TOKEN.kind)	
		return result;
	else
		error("Bad token: ", first);
}

Token Token_stream::get_token_after_SPACE() {
	Token t = get();
	while (t.kind == SPACE)
		t = get();
	if (t.kind == HELP)
		throw MANUAL_STRING; 

	return t;
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
		case '[': case ']': 
		case '{': case '}': 
		case '+': case '-': 
		case '*': case '/': case '%':
		case '=':  
		case print:
		case '!':
		case ',':
			return Token(ch);
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			input_stream.unget();
			return get_number();
		default:
			return get_other_token(ch);
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
		input_stream.get(ch);
	} while (input_stream && ch != c1 && ch != c2);

	return;
}

// Variable struct
struct Variable {
	string name;
	int value;
	bool constant;
	Variable(string n, int v) : name(n), value(v), constant(false) { }
	Variable(string n, int v, bool c) : name(n), value(v), constant(c) { }
};

// Symbol_table class has Variable struct vector and operations on this vector
//--------------------------------------------
class Symbol_table {
	private:
		vector<Variable> names;
	public:
		int get_value(string s);
		void set_value(string s, double d);
		bool is_declared(string s);
		int define_name(string s, double value, bool is_constant);
		void print_variables();
};

int Symbol_table::get_value(string s) {
	for (unsigned int i = 0; i < names.size(); ++i)
		if (names[i].name == s) 
			return names[i].value;
	error("get: undefined name ", s);
}

void Symbol_table::set_value(string s, double v) {
	if (! is_integer(v))
		error("set precondition: input parameter is not type int ");
	for (unsigned int i = 0; i < names.size(); ++i)
		if (names[i].name == s) {
			if (names[i].constant == true)
				error("set: constant name ", s);
			names[i].value = v;
			return;
		}
	error("set: undefined name ", s);
}

bool Symbol_table::is_declared(string s) {
	for (unsigned int i = 0; i < names.size(); ++i)
		if (names[i].name == s) 
			return true;
	return false;
}

int Symbol_table::define_name(string s, double value, bool is_constant) {
	if (! is_integer(value))
		error("set precondition: input parameter is not type int ");
	if (is_declared(s)) 
		error(s, " declared twice");
	names.push_back(Variable(s, value, is_constant));
	return value;
}

void Symbol_table::print_variables() {
	cout << "\n Already defined variables are listed below: \n";
	for (Variable x : names) {
		cout << x.name << " = " << x.value; 
		if (x.constant)
			cout << "    constant ";
		cout << endl;
	}
	cout << " Already defined variables are listed above\n";
}

// global variable of class Symbol_table with declared variables 
//--------------------------------------------
Symbol_table symbols;

// mathematical operations part 1
//--------------------------------------------
int factorial(double number) {
	if (0 > number)
		error("Precondition: can not calculate factorial for number < 0");
	if (! is_integer(number))
		error("Precondition: can not calculate factorial for NOT int type");

	int result = 1;
	for (short counter = 2; counter <= number; counter++) {
		if (result > INT_MAX / counter) {
			cerr << "Calculating factorial of " << number << " = " << number << "! :\n";
			cerr << "While calculating factorial of " << counter << " = " << counter << "! :\n";
			cerr << 	" result = " << result << " INT_MAX = " << INT_MAX << " \n";
			cerr << "result > INT_MAX / counter that is " << result << " > " << INT_MAX / counter << "\n";
			cerr << "result * counter = " << result * counter << "\n";
			throw overflow_error("integer overflow error");
		}
		result = result * counter;
	}
	
	if (0 >= result)
		throw runtime_error("Final condition: result of factorial can not be number <= 0");
		
	return result;
}

// declarations of primary in calculator
double primary(Token_stream& ts);

// mathematical operations part 2
//--------------------------------------------
int square_root(Token_stream& ts) {
	Token t = ts.get_token_after_SPACE();
	ts.unget(t);
	if ('(' != t.kind && '[' != t.kind && '{' != t.kind) 
		error("number of square root must be in brackets");
	
	double x = primary(ts);
	if (x < 0)
		error("Real solution for square root for number < 0 does not exist");
	if (false == is_integer(x))
		error("number of square root is not int type ");
	double result = sqrt(x);
	if (false == is_integer(result))
		error("result of square root is not int type ");
	
	return (int)result;
}

void print_power_overflow(int base, int exponent, const int LIMIT, int result, int counter) {
	string LIMIT_NAME;
	switch(LIMIT) {
		case INT_MAX:
			LIMIT_NAME = "INT_MAX";
			break;
		case INT_MIN:
			LIMIT_NAME = "INT_MIN";
			break;
		default:
			error("Unrecognized extremum ", LIMIT);
	}
	cerr << "Calculate " << base << " to power of " << exponent << " integer overflow :\n";
	cerr << "While calculating " << base << " to power of " << counter <<
		" result = " << result << " " << LIMIT_NAME << " = " << LIMIT << " \n";
	if (INT_MAX == LIMIT) {
		if (result > 0 && base > 0)
			cerr << "result > INT_MAX / base that is " << result << " > " << LIMIT / base << "\n";
		else if (result < 0 && base < 0)
			cerr << "result < INT_MAX / base that is " << result << " < " << LIMIT / base << "\n";
	}
	else {
		if (result < 0 && base > 0)
			cerr << "result < INT_MIN / base that is " << result << " < " << LIMIT / base << "\n";
		else if (result > 0 && base < 0)
			cerr << "result > INT_MIN / base that is " << result << " > " << LIMIT / base << "\n";
	}
		
	cerr << "result * base = " << result * base << "\n";
}

int power(int base, int exponent) {
	if (0 == base) {
		if (0 != exponent)
			return 0;
		error("0 to power of 0 is indeterminate form");
	}
	int result = 1;
	int last = exponent > 0 ? exponent : -exponent;
	
	//if (exponent < 0 && base != -1 && base != 1)
		//error("result of power is not int type ");
	
	for (int counter = 1; counter <= last; counter++) {
		if (result > 0 && base > 0) {
			if  (result > INT_MAX / base) {
				print_power_overflow(base, exponent, INT_MAX, result, counter);
				throw overflow_error("integer overflow error");
			}
		}
		else if (result < 0 && base < 0) {
			if (result < INT_MAX / base) {
				print_power_overflow(base, exponent, INT_MAX, result, counter);
				throw overflow_error("integer overflow error");
			}
		}
		else if (result < 0 && base > 0) {
			if (result < INT_MIN / base) {
				print_power_overflow(base, exponent, INT_MIN, result, counter);
				throw overflow_error("integer overflow error");
			}
		}
		else if (result > 0 && base < 0) {
			if (result > INT_MIN / base) {
				print_power_overflow(base, exponent, INT_MIN, result, counter);
				throw overflow_error("integer overflow error");
			}
		}
		
		result *= base;
	}
	
	if (exponent >= 0)
		return result;
	
	double result_double = 1. / result;
	if (false == is_integer(result_double))
		error("result of power is not int type ");
	result = result_double;
	
	return result;
}

// validate next token after token t to check calculator input data correction (to validate)
// in mathematical operations this function validate only sequence of input data
/* this function does not allow direct operations of the type: 
5!2 
6(7) 
(8)9
 which as a result create 2 separate expressions.
In order to create 2 separate expressions, separate these characters with a separator, e.g. new line, space or printing character:
5! 2 
6;(7)
(8) 9
*/
//--------------------------------------------
void validate_next_token(Token_stream& ts, Token& t) {
	Token next_token = ts.get();
	char next = next_token.kind;
	switch (t.kind) {
		case '!':
			if ('(' == next || '[' == next || '{' == next || '=' == next ||
				number == next || NAME == next || SQRT == next || '!' == next || POWER == next)
				error("Next token after factorial token can not be opening bracket or \
number or variable or sqrt or ! or power or =");
			break;
		case number:
			if ('(' == next || '[' == next || '{' == next ||
				NAME == next || SQRT == next || POWER == next)
				error("Next token after number can not be opening bracket or variable or sqrt or power");
			break;
		case ')':
		case ']':
		case '}':
			if ('(' == next || '[' == next || '{' == next ||
				number == next || NAME == next || SQRT == next || POWER == next)
				error("Next token after closed bracket can not be number or variable or \
opening bracket or sqrt or power");
			break;
		case NAME:
			if ('(' == next || '[' == next || '{' == next || SQRT == next || POWER == next)
				error("Next token after variable can not be opening bracket or sqrt or power");
			break;
		default:
			error("Next token should not be check for ", t.kind);
			
	}
	ts.unget(next_token);
}

// global variables to check calculator input data correction (to validate)
// in mathematical operations this global variables validate only sequence of input data
//--------------------------------------------
struct Braces_counter {
   short round_braces;   // counter opening round braces - curly braces can not be inside square or round brackets
   short square_braces;  // counter opening square braces - square braces can not be inside round brackets
   Braces_counter() : round_braces(0), square_braces(0) { }
};
Braces_counter braces_counter;

bool operation = false;      // can not accept sequence of +- +- ++ /- *+ *- and
// other mixes of 2 or more subsequent operators not separated by brackets

struct Assignment_validator {
   bool assignment_chance; // to signal assignment chance after detect first token as name of variable
   bool assignment_done;   // when assignment has done to signal assignment completed and 
   // other assignment can not exist in expression
   short names_counter;  // counter of variables to control nummber of variables before '='
   
   Assignment_validator() : 
      assignment_chance(false), assignment_done(false), names_counter(0) {}
};
Assignment_validator assignment_validator;

int expression(Token_stream& ts);

void token_error(const string& message, const Token& t);

char get_last_bracket_kind(const char bracket_kind) {
	switch(bracket_kind) {
		case '(':
			return ')';
		case '[':
			return ']';
		case '{':
			return '}';
		default:
			token_error("Unrecognized opening bracket ", bracket_kind);
	}
}

void increment_bracket_kind(const char bracket_kind) {
	switch(bracket_kind) {
		case '(':
			braces_counter.round_braces++;
			break;
		case '[':
			if (braces_counter.round_braces > 0) 
				error("Before close brace ), square brace [ is not accepted");
			braces_counter.square_braces++;
			break;
		case '{':
			if (braces_counter.square_braces > 0)
				error("Before close brace ], curly brace { is not accepted");
			if (braces_counter.round_braces > 0)
				error("Before close brace ), curly brace { is not accepted");
			break;
		default:
			token_error("Unrecognized opening bracket ", bracket_kind);
	}
}

void decrement_bracket_kind(const char bracket_kind) {
	switch(bracket_kind) {
		case '(':
			braces_counter.round_braces--;
			break;
		case '[':
			braces_counter.square_braces--;
			break;
		case '{':
			break;
		default:
			token_error("Unrecognized opening bracket ", bracket_kind);
	}
}

int power(Token_stream& ts) {
	int base, exponent;
	Token t = ts.get_token_after_SPACE();
	try {
		char bracket_kind = t.kind;
		char last_bracket = get_last_bracket_kind(bracket_kind);
		increment_bracket_kind(bracket_kind);
		base = expression(ts);
		t = ts.get_token_after_SPACE();
		if (',' != t.kind)
			error("in power calculation after base and before exponent must be ", ',');
		exponent = expression(ts);  
		t = ts.get_token_after_SPACE();
		if (t.kind != last_bracket) {
			ts.unget(t);
			error("closed bracket expected: ", last_bracket);
		}
		validate_next_token(ts, t); 
		decrement_bracket_kind(bracket_kind);
	}
	catch(runtime_error& e) {
		if(t.kind == NEW_LINE)
			ts.unget(t);
		error(e.what() + string("\nIn power calculation base, ',' and exponent must be in only one pair of brackets \n"),
			"Syntax: pow(base, exponent) - base and exponent may be expression in brackets but \n\
base, ',' and exponent must be in only one pair of brackets after name of pow");
	}

	int result = power(base, exponent);
	return result;
}

int brackets_expression(Token_stream& ts, const char bracket_kind) {
	char last_bracket = get_last_bracket_kind(bracket_kind);
	increment_bracket_kind(bracket_kind);
	int result = expression(ts);
	Token t = ts.get_token_after_SPACE();
	if (t.kind != last_bracket) {
		ts.unget(t);
		error("closed bracket expected: ", last_bracket);
	}
	validate_next_token(ts, t); 
	decrement_bracket_kind(bracket_kind);

   return result;
}

void token_error(const string& message, const Token& t) {
	string name = get_name(t);
	if (name.size() > 0)
		error(message, name);
	else if (NAME == t.kind)
		error(message);
	else
		error(message, t.kind);
}

// calculator functions to token process and calculations
//-------------------------------------------- 
double primary(Token_stream& ts) {
	double result;
	Token t = ts.get_token_after_SPACE();
	switch (t.kind) {
	case '(':
	case '[':
	case '{':
		result = brackets_expression(ts, t.kind);
		operation = false;
		break;
	case '-':
	case '+':
		if (operation)
			error("Next token after operator can not be + or -");
		operation = true;
		result = '+' == t.kind ? primary(ts) : -primary(ts);
		break;
	case NEW_LINE:   // to allow many lines calculations without ignoring new line tokens in input_stream and Token_stream
		return primary(ts);  // return before end of method to avoid set assignment_chance to false for first entered name
								// to allow assignment divided in many lines
	case number:
		result = t.value;
		operation = false;
		validate_next_token(ts, t);
		break;
	case NAME:
		result = symbols.get_value(t.name);
		operation = false;
		validate_next_token(ts, t);
		if(assignment_validator.assignment_chance)
			assignment_validator.names_counter++;
		return result;    // return before end of method to avoid set assignment_chance to false for first entered name
			
	default:
		token_error("unrecognized primary: ", t);
	}
	if(assignment_validator.assignment_chance)
		assignment_validator.assignment_chance = false;   // after get tokens other than NAME, assignment_chance is set to false
		
	return result;
}

int factor(Token_stream& ts);

// before primary may be minus or sqrt
// skip plus because is not problematic for factorial calculations
// which order of operations has precedence over * / % - + 
double before_primary(Token_stream& ts, Token& t, bool& minus_number) {
	double result;
	switch (t.kind) {
		case SQRT:
			result = square_root(ts);
			break;
		case POWER:
			result = power(ts);
			break;
		case '-':    // to allow minus '-' as first token in expression with factorial, which can not accept minus numbers
						 // and generate errors for -4! == -24  and  (-4)!
			if (operation)
				error("Next token after operator can not be + or -");
			minus_number = true;    // ok: -4! == -24    error: (-4)! 
			operation = true;
			t = ts.get_token_after_SPACE();
			result = before_primary(ts, t, minus_number);
			break;
		default: 
			ts.unget(t);
			result = primary(ts);
			if (false == minus_number && result == -(long long)(INT_MIN))   // absolute value of labs(INT_MIN) == labs(INT_MAX + 1)
				error("Max number for int is ", INT_MAX);
			break;
	}
	return result;
}

// after primary may be tokens: power(exponent), factorial and assignment '='
// which order of operations has precedence over * / % - + 
double after_primary(Token_stream& ts, double x, Token& t) {
	double result;
	switch (t.kind) {
		case '!':    // to allow minus '-' as first char in expression with factorial
			if (operation)
				error("Next token after operator can not be '!'");
			result = factorial(x);  // -4! == -24    (-4)! error
			validate_next_token(ts, t);
			break;
		case '=': 
		if (! assignment_validator.assignment_chance || assignment_validator.assignment_done 
            || assignment_validator.names_counter > 1)
			error("Assignment operator must be only one in expression and \n must occur after variable name directly \
and if variable name is first primary in expression ");
				assignment_validator.assignment_done = true;
				assignment_validator.assignment_chance = false;
				result = expression(ts);
				break;
		default:
			ts.unget(t);
			result = x;
	}
	return result;
}

// calculate of factors (square root, power, factorial)
// which order of operations has precedence over * / % - + 
int factor(Token_stream& ts) {
	bool minus_number = false;
	Token t = ts.get_token_after_SPACE();
	double result = before_primary(ts, t, minus_number);
	t = ts.get_token_after_SPACE();
	result = after_primary(ts, result, t);
	if (minus_number)
		result = -result;
	if (false == is_integer(result))
		error("result is not int type");
	return (int)result;
}

// calculate of elements / % * (quotient, modulo quotient, product)
// which order of operations has precedence over - + 
int term(Token_stream& ts) {
	double left = factor(ts);
	operation = true;
	while (true) {
		Token t = ts.get_token_after_SPACE();
		switch (t.kind) {
		case '*':
			left *= factor(ts);
			if (false == is_integer(left))
				error("result of multiplication is not int type ");
			break;
		case '/':
		case '%': {	
			int f = factor(ts);
			if (f == 0) 
				error("divide by zero");
			if (t.kind == '%') 
				left = (int)left % f;
			else {
				 left /= f;
				 if (false == is_integer(left))
					error("result of division is not integer ");
			}	
			break;
		}
		default:
			ts.unget(t);
			return int (left);
		}
	}
}

// calculate of elements sums and differences
// which order of operations is the lowest
int expression(Token_stream& ts) {
	operation = false;
	double left = term(ts);
	operation = true;
	while (true) {
		Token t = ts.get_token_after_SPACE();
		switch (t.kind) {
		case '+':
			left += term(ts);
			if (false == is_integer(left))
				error("result of addition is not int type ");
			break;
		case '-':
			left -= term(ts);
			if (false == is_integer(left))
				error("result of subtraction is not int type ");
			break;
		default:
			ts.unget(t);
			return (int) left;
		}
	}
}

void reset_statement_status() {
	braces_counter.round_braces = 0;   // set counter opening round braces to 0 to cleaning before next operations
	braces_counter.square_braces = 0;  // set counter opening square braces to 0 to cleaning before next operations
	assignment_validator.assignment_chance = false;
	assignment_validator.assignment_done = false;
	assignment_validator.names_counter = 0;
}

int declaration(Token_stream& ts) {
	Token t = ts.get_token_after_SPACE();
	bool is_constant = ts.is_constant_token(t);
	ts.check_name(t);
	Token t2 = ts.get_token_after_SPACE();
	if (t2.kind != '=') {
		ts.unget(t2);
		error("= must be directly after only one variable name in declaration of ", t.name);
	}
	int value = expression(ts);
	symbols.define_name(t.name, value, is_constant);
	return value;
}

int statement(Token_stream& ts) {
	reset_statement_status();
	Token t = ts.get_token_after_SPACE();
	switch (t.kind) {
		case declaration_key:
			try {
				return declaration(ts);
			}
			catch (string&) {
				error(HELP_NAME, " are keywords and can not be used as variable");
			}
		case NAME: {
			assignment_validator.assignment_chance = true;
			ts.unget(t);
			int value = expression(ts);
			if (assignment_validator.assignment_done) 
				symbols.set_value(t.name, value);
			return value;
		}
		default:
			ts.unget(t);
			return expression(ts);
	}
}

void clean_up_mess(Token_stream& ts) {
	ts.ignore(print, NEW_LINE);
	reset_statement_status();
}

bool is_running(Token_stream& ts, const string prompt) {
	Token t = EMPTY_TOKEN;
	bool skipping = true;
	do {
		t = ts.get();
		switch (t.kind) {
			case HELP:
				cout << MANUAL_STRING;
				cout << prompt;
				break;
			case VARS_VIEW:
				symbols.print_variables();
				cout << prompt;
			case NEW_LINE:
			case SPACE:
			case print:
				break;
			default:
				skipping = false;
		}
	} while(skipping);				
	if (t.kind == quit) 
		return false;
	else 
		ts.unget(t);
	return true;
}

void calculate(Token_stream& ts) {
   const string prompt = "> ";
   const string result = "= ";
	int value = INT_MIN;
	do {
		try {
			cout << prompt;
			if (false == is_running(ts, prompt))
				return;
			value = statement(ts);             // separation in call of statement() and
			cout << result << value << endl; // call of cout to do not allow display string of result 
			                                 // before end of writing many lines calculations
		} 
		catch (runtime_error& e) {
			cerr << e.what() << endl;
			clean_up_mess(ts);
		}
		catch (string& m) {
			system("clear");
			cout << m;
			clean_up_mess(ts);
		}
	} while (true);
}

void enter_key(Token_stream& ts, char key) {
	cout << "Enter \'" << key << "\' to continue ";
   char c;
   do {
      c = ts.get_char();
   } while (c != key);
}

void add_predefined_variables() {
	symbols.define_name("k", 1000, false);
}

int main() {
   Token_stream ts(cin);
   try {
      cout << "Welcome in simple calculator for only int type.\n";
      cout << MANUAL_STRING;
      add_predefined_variables();
      symbols.print_variables();
      calculate(ts);
      return 0;
   }
   catch (exception& e) {
      cerr << "exception: " << e.what() << endl;
      enter_key(ts, print);
      return 1;
   }
   catch (...) {
      cerr << "unrecognized exception\n";
      enter_key(ts, print);
      return 2;
   }
}
