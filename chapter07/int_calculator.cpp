#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <limits>

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

inline void error(const string& s, long i) {
	cerr << "!!!!! Error: " << s << i <<  endl;
   throw runtime_error("");
}

bool is_integer(double x) {
	int integer = x;
	if (integer != x) {
		cerr << "Parameter double x = " << x << endl;
		cerr << "Int of x = " << integer << endl;
		return false;
		//error("Parameter is NOT int type");
	}
	return true;
}

bool is_integer(long long x) {
	int integer = x;
	if (integer != x) {
		cerr << "Parameter long long x = " << x << endl;
		cerr << "Int of x = " << integer << endl;
		return false;
		//error("Parameter is NOT int type");
	}
	return true;
}

bool is_integer(long x) {
	int integer = x;
	if (integer != x) {
		cerr << "Parameter long x = " << x << endl;
		cerr << "Int of x = " << integer << endl;
		return false;
		//error("Parameter is NOT int type");
	}
	return true;
}

void check_integer_range(long long val) {
	cerr << " val = " << val << endl;
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
const string quit_name = "quit";
const string POWER_NAME = "pow";
const string CONST_NAME = "const";
const string VARS_VIEW_NAME = "vars_view";
const string SQRT_NAME = "sqrt";
const string HELP_NAME = string(1, toupper(HELP)) + " or " + string(1, tolower(HELP));

// Token program keywords can not be used as variable name
const vector<char> NOT_VARIABLES_TOKENS = { quit, SQRT, HELP, print, POWER, declaration_key,
														CONST, VARS_VIEW };

bool exist(char c, const vector<char>& vec) {
	for (char a : vec) 
		if (a == c) 
			return true;		
	return false;
}

const vector<string> NOT_VARIABLES_NAMES = { quit_name, SQRT_NAME, CONST_NAME, VARS_VIEW_NAME,
											string(1, toupper(HELP)), string(1, tolower(HELP)) };

bool exist(string s, const vector<string>& vec) {
	for (string a : vec) 
		if (a == s) 
			return true;		
	return false;
}

// Token struct
//--------------------------------------------
struct Token {
	char kind;
	int value;
	string name;
	
	Token(char ch) :kind(ch), value(0) { }
	Token(char ch, int val) :kind(ch), value(val) { }
	Token(char ch, string s) :kind(ch), name(s) { }
};

string get_reserved_name(const Token& t) {
	char kind = t.kind;
	switch (kind) {
		case quit:
			return quit_name;
		case SQRT:
			return SQRT_NAME;
		case HELP:
			return HELP_NAME;
		case print:
			return string(1, print);
		case POWER:
			return string(1, POWER);
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

struct Manual {
	string manual_string = string("MANUAL for simple calculator. \n\
Using only int type numbers and variables for calculation.\n\
Max number for int is ") + to_string(INT_MAX) +
string(". Min number for int is ") + to_string(INT_MIN) +
string("\nIn below informations 'x', 'y', 'z' are treated as number or variable \n\
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
7. power(exponentiation)) x") + string(1, POWER) + "y (x to power of y - y is exponent) \n\
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
To display this manual enter " + string(1, HELP) + " case insensitive \n";
};

Manual manual;

char get_char() {
	char ch;
	cin.get(ch);
	if(!cin) {
		cout << "Exit \n";
		exit(0);
	}
	return ch;
}

// Token_stream class
//--------------------------------------------
class Token_stream {
private:
	bool full;
	Token buffer;
	
	Token get_number();
	Token get_single_char_token(char first);
	Token get_alphanum_token(char first);
	Token get_other_token(char c);
public:
	Token_stream() :full(false), buffer(0) { }
	
	Token get();
	void unget(Token& t);
	void ignore(char, char);
	Token get_token_after_SPACE();
	void check_name(Token& t);
	bool is_constant_token(Token& t);
	string get_name_string(char first);
	string get_name_string();
};

string Token_stream::get_name_string() {
	string s = "";
	char c;
	while (cin.get(c) && ('_' == c || isalpha(c) || isdigit(c)))
		s += c;
			
	cin.unget();   // to save non-alphanumerical character in input stream
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
	long long val;
	cin >> val;
	if (!cin)
		error("Attempt to reading number has failed");
	if (val != -(long long)(INT_MIN) && false == is_integer(val))  // absolute value of llabs(INT_MIN) == llabs(INT_MAX + 1)
		error("input value is not int type ");
	
	return Token(number, val);
}

Token EMPTY_TOKEN = Token(0);  // to signal that get of token is unsuccessfull

Token Token_stream::get_single_char_token(char first) {
	if (isspace(first)) 
		return Token(SPACE);
		
	if (first != declaration_key && first != tolower(HELP) && first != toupper(HELP))
		return EMPTY_TOKEN; // not single token
		
	Token result = EMPTY_TOKEN;
	char next = get_char();   // get next char after 'first' char
	cin.unget();   // to save 'next' character in input stream
	if (first == declaration_key) {
		if (next != '\n' && isspace(next))
			result = Token(declaration_key);
		else
			error("After declaration key must be whitespace except new line");
	}
	else if (first == tolower(HELP) || first == toupper(HELP)) {
		if (next == print || isspace(next))
			result = Token(HELP);
		else if (! isalnum(next))
			error("After help key must be whitespace or " + string(1, print) + " or alphanumerical key");
	}
		
	return result;
}

Token Token_stream::get_alphanum_token(char first) {
	string s = get_name_string(first);
	if (s == quit_name) 
		return Token(quit);
	else if (s == SQRT_NAME) 
		return Token(SQRT);
	else if (s == CONST_NAME) 
		return Token(CONST);
	else if (s == VARS_VIEW_NAME) 
		return Token(VARS_VIEW);
	else if (s.size() > 0) 
		return Token(NAME, s);
		
	return EMPTY_TOKEN;
}

void Token_stream::check_name(Token& t) {
	if (exist(t.kind, NOT_VARIABLES_TOKENS)) {
		string keyword_name = get_reserved_name(t);
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
			error(CONST_NAME + " is keyword and can not be used as variable. \n",
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
		throw manual; 

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
		case POWER:
			return Token(ch);
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			cin.unget();
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
		cin.get(ch);
	} while (cin && ch != c1 && ch != c2);

	return;
}

// global variable of type Token_stream 
//--------------------------------------------
Token_stream ts;

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
		void set_value(string s, int d);
		bool is_declared(string s);
		int define_name(string s, int value, bool is_constant);
		void print_variables();
};

int Symbol_table::get_value(string s) {
	for (unsigned int i = 0; i < names.size(); ++i)
		if (names[i].name == s) 
			return names[i].value;
	error("get: undefined name ", s);
}

void Symbol_table::set_value(string s, int d) {
	for (unsigned int i = 0; i <= names.size(); ++i)
		if (names[i].name == s) {
			if (names[i].constant == true)
				error("set: constant name ", s);
			names[i].value = d;
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

int Symbol_table::define_name(string s, int value, bool is_constant) {
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
int factorial(int number) {
	if (0 > number)
		error("Precondition: can not calculate factorial for number < 0");
	//if (! is_integer(number))
		//error("Precondition: can not calculate factorial for NOT int type");

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
long primary();

// mathematical operations part 2
//--------------------------------------------
int square_root() {
	Token t = ts.get_token_after_SPACE();
	ts.unget(t);
	if ('(' != t.kind && '[' != t.kind && '{' != t.kind) 
		error("number of square root must be in brackets");
	
	long x = primary();
	if (x < 0)
		error("Real solution for square root for number < 0 does not exist");
	if (false == is_integer(x))
		error("number of square root is not int type ");
	double result_double = sqrt(x);
	if (false == is_integer(result_double))
		error("result of square root is not int type ");
	int result = result_double;
	return result;
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
			cerr << "result > INT_MAX / base that is " << result << " > " << INT_MAX / base << "\n";
		else if (result < 0 && base < 0)
			cerr << "result < INT_MAX / base that is " << result << " < " << INT_MAX / base << "\n";
	}
	else {
		if (result < 0 && base > 0)
			cerr << "result < INT_MIN / base that is " << result << " < " << INT_MIN / base << "\n";
		else if (result > 0 && base < 0)
			cerr << "result > INT_MIN / base that is " << result << " > " << INT_MIN / base << "\n";
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

int power(int base) {
	Token t = ts.get_token_after_SPACE();
	ts.unget(t);
	if ('(' != t.kind && '[' != t.kind && '{' != t.kind)
		error("in power calculation exponent must be in brackets");
	long exponent = primary();  // brackets are in primary()
	if (false == is_integer(exponent))
		error("exponent is not int type ");
	int result = power(base, (int)exponent);
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
void validate_next_token(Token& t) {
	Token next_token = ts.get();
	char next = next_token.kind;
	switch (t.kind) {
		case '!':
			if ('(' == next || '[' == next || '{' == next || '=' == next ||
				number == next || NAME == next || SQRT == next || '!' == next)
				error("Next token after factorial token can not be opening bracket or \
number or variable or sqrt or ! or =");
			break;
		case number:
			if ('(' == next || '[' == next || '{' == next ||
				NAME == next || SQRT == next)
				error("Next token after number can not be opening bracket or variable or sqrt");
			break;
		case ')':
		case ']':
		case '}':
			if ('(' == next || '[' == next || '{' == next ||
				number == next || NAME == next || SQRT == next)
				error("Next token after closed bracket can not be number or variable or \
opening bracket or sqrt");
			break;
		case NAME:
			if ('(' == next || '[' == next || '{' == next || SQRT == next)
				error("Next token after variable can not be opening bracket or sqrt");
			break;
		default:
			error("Next token should not be check for ", t.kind);
			
	}
	ts.unget(next_token);
}

// global variables to check calculator input data correction (to validate)
// in mathematical operations this global variables validate only sequence of input data
//--------------------------------------------
short round_braces = 0;   // counter opening round braces - curly braces can not be inside square or round brackets
short square_braces = 0;  // counter opening square braces - square braces can not be inside round brackets

bool operation = false;      // can not accept sequence of +- +- ++ /- *+ *- and
// other mixes of 2 or more subsequent operators not separated by brackets

bool assignment_chance = false; // to signal assignment chance after detect first token as name of variable
bool assignment_done = false;   // when assignment has done to signal assignment completed and 
// other assignment can not exist in expression
short names_counter = 0;  // counter of variables to control nummber of variables before '='

int expression();

int brackets_expression(char bracket_kind) {
	char last_bracket = 0;
	switch(bracket_kind) {
		case '(':
			last_bracket = ')';
			round_braces++;
			break;
		case '[':
			if (round_braces > 0) 
				error("Before close brace ), square brace [ is not accepted");
			last_bracket = ']';
			square_braces++;
			break;
		case '{':
			if (square_braces > 0)
				error("Before close brace ], curly brace { is not accepted");
			if (round_braces > 0)
				error("Before close brace ), curly brace { is not accepted");
			last_bracket = '}';
			break;
		default:
			error("Unrecognized opening bracket ", bracket_kind);
	}
		
	int result = expression();
	Token t = ts.get_token_after_SPACE();
	if (t.kind != last_bracket) {
		ts.unget(t);
		error("closed bracket expected: ", last_bracket);
	}
	validate_next_token(t); 
	if (bracket_kind == '[')
		square_braces--;
	else if (bracket_kind == '(')
		round_braces--;

   return result;
}

void not_primary_error(const Token& t) {
	string name = get_reserved_name(t);
	if (name.size() > 0)
		error("unrecognized primary: ", name);
	else
		error("unrecognized primary: ", t.kind);
}

// calculator functions to token process and calculations
//-------------------------------------------- 
long primary() {
	long result;
	Token t = ts.get_token_after_SPACE();
	switch (t.kind) {
	case '(':
	case '[':
	case '{':
		result = brackets_expression(t.kind);
		operation = false;
		break;
	case '-':
	case '+':
		if (operation)
			error("Next token after operator can not be + or -");
		operation = true;
		result = '+' == t.kind ? primary() : -primary();
		break;
	case NEW_LINE:   // to allow many lines calculations without ignoring new line tokens in cin and Token_stream
		return primary();  // return before end of method to avoid set assignment_chance to false for first entered name
								// to allow assignment divided in many lines
	case number:
		result = t.value;
		operation = false;
		validate_next_token(t);
		break;
	case NAME:
		result = symbols.get_value(t.name);
		operation = false;
		validate_next_token(t);
		if(assignment_chance)
			names_counter++;
		return result;    // return before end of method to avoid set assignment_chance to false for first entered name
			
	default:
		not_primary_error(t);
	}
	if(assignment_chance)
		assignment_chance = false;   // after get tokens other than NAME, assignment_chance is set to false
		
	return result;
}

long factor();

// before primary may be minus or sqrt
// skip plus because is not problematic for factorial calculations
// which order of operations has precedence over * / % - + 
long before_primary(Token& t, bool& minus_number) {
	long result;
	switch (t.kind) {
		case SQRT:
			result = square_root();
			break;
		case '-':    // to allow minus '-' as first token in expression with factorial, which can not accept minus numbers
						 // errors: -4! == -24  and  (-4)!
			if (operation)
				error("Next token after operator can not be + or -");
			minus_number = true;    // ok: -4! == -24    error: (-4)! 
			operation = true;
			result = -factor();
			if (result == -(long)(INT_MIN))   // absolute value of llabs(INT_MIN) == llabs(INT_MAX + 1)
				error("Max number for int is ", INT_MAX);
			break;
		default: 
			ts.unget(t);
			result = primary();
			break;
	}
	if (result != -(long long)(INT_MIN) && false == is_integer(result))  // absolute value of llabs(INT_MIN) == llabs(INT_MAX + 1)
		error("value is not int type ", result);
	return result;
}

// after primary may be tokens: power(exponent), factorial and assignment '='
// which order of operations has precedence over * / % - + 
int after_primary(int x, Token& t) {
	int result;
	switch (t.kind) {
		case POWER:
			if (operation)   // to avoid sequence type: +^ /^ 
				error("Next token after operator can not be ", POWER);
			result = power(x);
			break;
		case '!':    // to allow minus '-' as first char in expression with factorial
			if (operation)
				error("Next token after operator can not be '!'");
			result = factorial(x);  // -4! == -24    (-4)! error
			validate_next_token(t);
			break;
		case '=': 
		if (! assignment_chance || assignment_done || names_counter > 1)
			error("Assignment operator must be only one in expression and \n must occur after variable name directly \
and if variable name is first primary in expression ");
				assignment_done = true;
				assignment_chance = false;
				result = expression();
				break;
		default:
			ts.unget(t);
			result = x;
	}
	return result;
}

// calculate of factors (square root, power, factorial)
// which order of operations has precedence over * / % - + 
long factor() {
	bool minus_number = false;
	Token t = ts.get_token_after_SPACE();
	long result = before_primary(t, minus_number);
	t = ts.get_token_after_SPACE();
	result = after_primary(result, t);
	//if (minus_number && result == -(long)(INT_MIN))   // absolute value of llabs(INT_MIN) == llabs(INT_MAX + 1)
		//error("Min number for int is ", INT_MIN);
	if (minus_number)
		result = -result;
	return result;
}

// calculate of elements / % * (quotient, modulo quotient, product)
// which order of operations has precedence over - + 
int term() {
	double left = factor();
	operation = true;
	while (true) {
		Token t = ts.get_token_after_SPACE();
		switch (t.kind) {
		case '*':
			left *= factor();
			if (false == is_integer(left))
				error("result of multiplication is not int type ");
			break;
		case '/':
		case '%': {	
			int f = factor();
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
int expression() {
	operation = false;
	double left = term();
	operation = true;
	while (true) {
		Token t = ts.get_token_after_SPACE();
		switch (t.kind) {
		case '+':
			left += term();
			if (false == is_integer(left))
				error("result of addition is not int type ");
			break;
		case '-':
			left -= term();
			if (false == is_integer(left))
				error("result of subtraction is not int type ");
			break;
		default:
			ts.unget(t);
			return (int) left;
		}
	}
}

void reset_global_variables() {
	round_braces = 0;   // set counter opening round braces to 0 to cleaning before next operations
	square_braces = 0;  // set counter opening square braces to 0 to cleaning before next operations
	assignment_chance = false;
	assignment_done = false;
	names_counter = 0;
}

int declaration() {
	Token t = ts.get_token_after_SPACE();
	bool is_constant = ts.is_constant_token(t);
	ts.check_name(t);
	Token t2 = ts.get_token_after_SPACE();
	if (t2.kind != '=') {
		ts.unget(t2);
		error("= must be directly after only one variable name in declaration of ", t.name);
	}
	int value = expression();
	symbols.define_name(t.name, value, is_constant);
	return value;
}

int statement() {
	reset_global_variables();
	Token t = ts.get_token_after_SPACE();
	switch (t.kind) {
		case declaration_key:
			try {
				return declaration();
			}
			catch (Manual&) {
				error(HELP_NAME, " are keywords and can not be used as variable");
			}
		case NAME: {
			assignment_chance = true;
			ts.unget(t);
			int value = expression();
			if (assignment_done) 
				symbols.set_value(t.name, value);
			return value;
		}
		default:
			ts.unget(t);
			return expression();
	}
}

void clean_up_mess() {
	ts.ignore(print, NEW_LINE);
	reset_global_variables();
}

const string prompt = "> ";
const string result = "= ";

bool is_running() {
	Token t = EMPTY_TOKEN;
	bool skipping = true;
	do {
		t = ts.get();
		switch (t.kind) {
			case HELP:
				cout << manual.manual_string;
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

void calculate() {
	int value = INT_MIN;
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
		catch (Manual& m) {
			system("clear");
			cout << manual.manual_string;
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

void add_predefined_variables() {
	symbols.define_name("k", 1000, false);
}

int main()
try {
	cout << "Welcome in simple calculator for only int type.\n";
	cout << manual.manual_string;
	add_predefined_variables();
	symbols.print_variables();
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
