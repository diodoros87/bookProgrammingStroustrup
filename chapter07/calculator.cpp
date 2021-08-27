#include <iostream>
#include <vector>
#include <cmath>
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

// String and char operations
//--------------------------------------------
string get_name_string(char first) {
	string s = "";
	if (isalpha(first) || '_' == first) {   // string with only letter at start
		s += first;          // get alphanumerical string
		char c;
		while (cin.get(c) && ('_' == c || isalpha(c) || isdigit(c)))
			s += c;
			
		cin.unget();   // to save non-alphanumerical character in input stream
	}
	return s;
}

char get_char() {
	char ch;
	cin.get(ch);
	if(!cin) {
		cout << "Exit \n";
		exit(0);
	}
	return ch;
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

constexpr char SQUARE_ROOT = 'r';
constexpr char EXPONENTIATION = 'p';

// Token program keywords
//--------------------------------------------
const string quit_name = "quit";
const string CONST_NAME = "const";
const string SQRT_NAME = "sqrt";
const string HELP_NAME = string(1, toupper(HELP)) + " or " + string(1, tolower(HELP));

// Token program keywords can not be used as variable name
const vector<char> NOT_VARIABLES_TOKENS = { quit, SQRT, HELP, print, POWER, declaration_key, CONST };

bool exist(char c, const vector<char>& vec) {
	for (char a : vec) 
		if (a == c) 
			return true;		
	return false;
}

const vector<string> NOT_VARIABLES_NAMES = { quit_name, SQRT_NAME, CONST_NAME,
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
	double value;
	string name;
	
	Token(char ch) :kind(ch), value(0) { }
	Token(char ch, double val) :kind(ch), value(val) { }
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
		default:
			return "";
	}
}

class Manual {
	public:
		void print_help();
};

void Manual::print_help() {
	cout << "MANUAL for simple calculator.\n ";
	cout << "Using floating-point numbers and variables for calculation." << endl;
	cout << "In below informations 'x', 'y', 'z' are treated as number or variable \n";
	cout << "Signed numbers or variables -x (-x) +x (+x) are allowed \
but --x x++ -+x are unacceptable\n";
	cout << "To their accept necessary is separation by brackets -(-x) +(-y) \
but every sign must be separated by number or bracket \n";
	cout << "Operators can not follow each other - between operators must be bracket or variable or number:\n";
	cout << "+x*y-z*(-x/t) is OK, but x/-z is unacceptable \n";
	cout << "Supported set of brackets: \n";
	cout << "{} [] {[]} () [()] {()} {[()]} \n";
	cout << "Each kind of brackets may be inside the same kind of bracket: (()) [[]] {{{}}} \n";
	cout << "{} can not be be inside () and []  \n";
	cout << "[] can not be be inside ()  \n";
	cout << "Supported operations: \n";
	cout << "1. addition x+y \n";
	cout << "2. subtraction x-y \n";
	cout << "3. multiplication x*y \n";
	cout << "4. division x/y \n";
	cout << "5. modulo division x%y \n";
	cout << "6. square root sqrt(x) - number of sqrt must be in any supported bracket kind \n";
	cout << "7. power(exponentiation)) x^y (x to power of y - y is exponent) \n";
	cout << "number of exponent must be in any supported bracket kind \n";
	cout << "8. factorial x! \n";
	cout << "To separate many calculations press " << print << " or whitespace or new line " << endl;
	cout << "To do operation in many lines enter operator directly before press new line for example 2+3+ENTER: \n";
	cout << "In new line operation will be continue \n";
	cout << "To define new variable enter "<< declaration_key << " whitespace, variable name = value(number or other variable)\n";
	cout << "Variable name must started with letter or _ and may contain letters, digits and _ \n";
	cout << "Can not declare identical variable name \n";
	cout << "To define constant 'variable' enter: " << declaration_key << " " << CONST_NAME << " name_of_variable \n";
	cout << "To quit enter "<< quit_name << endl;
	cout << "To display this manual enter " << HELP << " case insensitive " << endl;
}

Manual manual;

// Token_stream class
//--------------------------------------------
class Token_stream {
private:
	bool full;
	Token buffer;
	
	Token get_number();
	Token get_number_with_dot();
	Token get_single_token(char first);
	Token get_alphanum_token(char first);
	Token get_other_token(char c);
public:
	Token_stream() :full(false), buffer(0) { }
	
	Token get();
	void unget(Token& t);
	void ignore(char, char);
	Token get_token_after_SPACE();
	bool is_constant_token(Token& t);
};

void Token_stream::unget(Token& t) {
	if (full) 
		error("unget() into a full buffer");
	buffer = t;       // copy t to buffer
	full = true;      // buffer is now full
}

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

Token EMPTY_TOKEN = Token(0);

Token Token_stream::get_single_token(char first) {
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
	else if ((first == tolower(HELP) || first == toupper(HELP)) &&
				(next == print || isspace(next)))
		result = Token(HELP);
		
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
	else if (s.size() > 0)
		return Token(NAME, s);
		
	return EMPTY_TOKEN;
}

bool Token_stream::is_constant_token(Token& t) {
	bool result = false;
	if (t.kind == CONST) { 
		result = true;
		t = get_token_after_SPACE();
		if (t.kind != NAME)
			error(CONST_NAME + " is keyword and can not be used as variable. ",
			"After this keyword must be new declared name");
	}
	return result;
}

Token Token_stream::get_other_token(char first) {
	Token result = get_single_token(first);
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
		case '.':
			return get_number_with_dot();
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
	double value;
	bool constant;
	Variable(string n, double v) : name(n), value(v), constant(false) { }
	Variable(string n, double v, bool c) : name(n), value(v), constant(c) { }
};

// Variable structs vector and operations on this vector
//--------------------------------------------
class Symbol_table {
	private:
		vector<Variable> names;
	public:
		double get_value(string s);
		void set_value(string s, double d);
		bool is_declared(string s);
		double define_name(string s, double value, bool is_constant);
		void print_variables();
};

double Symbol_table::get_value(string s) {
	for (unsigned int i = 0; i < names.size(); ++i)
		if (names[i].name == s) 
			return names[i].value;
	error("get: undefined name ", s);
}

void Symbol_table::set_value(string s, double d) {
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

double Symbol_table::define_name(string s, double value, bool is_constant) {
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

// global variable of type Symbol_table with declared variables 
//--------------------------------------------
Symbol_table symbols;

// mathematical operations part 1
//--------------------------------------------
bool is_integer(double x) {
	int integer = x;
	if (integer == x)
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

// declarations of primary in calculator
double primary();

// mathematical operations part 2
//--------------------------------------------
double square_root() {
	Token t = ts.get_token_after_SPACE();
	ts.unget(t);
	if ('(' != t.kind && '[' != t.kind && '{' != t.kind) 
		error("number of square root must be in brackets");
	
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
	ts.unget(t);
	if ('(' != t.kind && '[' != t.kind && '{' != t.kind)
		error("in power calculation exponent must be in brackets");
	double exponent = primary();  // brackets are in primary()
	if (false == is_integer(exponent))
		error("calculate of power only for integer exponent");
	double result = power(base, exponent);
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
			if ('(' == next || '[' == next || '{' == next ||
				number == next || NAME == next || SQRT == next || '!' == next)
				error("Next token after factorial token can not be opening bracket or \
number or variable or sqrt or !");
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
bool round_braces = false;   // curly braces can not be inside square or round brackets
bool square_braces = false;  // square braces can not be inside round brackets
bool operation = false;      // can not accept sequence of +- +- ++ /- *+ *- and
// other mixes of 2 or more subsequent operators not separated by brackets
bool assignment_chance = false; // to signal assignment chance after detect first token as name of variable
bool assignment_done = false;   // when assignment has done to signal assignment completed and 
// other assignment can not exist in expression

double expression();

double brackets_expression(char bracket_kind) {
	char last_bracket = 0;
	switch(bracket_kind) {
		case '(':
			last_bracket = ')';
			round_braces = true;
			break;
		case '[':
			if (round_braces) 
				error("Before close brace ), square brace [ is not accepted");
			last_bracket = ']';
			square_braces = true;
			break;
		case '{':
			if (square_braces)
				error("Before close brace ], curly brace { is not accepted");
			if (round_braces)
				error("Before close brace ), curly brace { is not accepted");
			last_bracket = '}';
			break;
		default:
			error("Unrecognized opening bracket ", bracket_kind);
	}
		
	double result = expression();
	Token t = ts.get_token_after_SPACE();
	if (t.kind != last_bracket) {
		ts.unget(t);
		error("closed bracket expected: ", last_bracket);
	}
	validate_next_token(t); 
	if (bracket_kind == '[')
		square_braces = false;
	else if (bracket_kind == '(')
		round_braces = false;
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
double primary() {
	double result;
	Token t = ts.get_token_after_SPACE();
	switch (t.kind) {
	case '(':
	case '[':
	case '{':
		result = brackets_expression(t.kind);
		operation = false;
		break;
	case '+':
	case '-':
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
		return result;    // return before end of method to avoid set assignment_chance to false for first entered name
			
	default:
		not_primary_error(t);
	}
	if(assignment_chance)
		assignment_chance = false;   // after get token other than NAME, assignment_chance is set ot false
	return result;
}

double factor();

// before primary may be minus or sqrt
// skip plus because is not problematic for factorial calculations
// which order of operations has precedence over * / % - + 
double before_primary(Token& t, bool& minus_number) {
	switch (t.kind) {
		case SQRT:
			return square_root();
		case '-':    // to allow minus '-' as first token in expression with factorial, which can not accept minus numbers
						 // errors: -4! == -24  and  (-4)!
			if (operation)
				error("Next token after operator can not be + or -");
			minus_number = true;    // ok: -4! == -24    error: (-4)! 
			operation = true;
			return factor();
		default:
			ts.unget(t);
			return primary();
	}
}

// after primary may be tokens: power(exponent) and factorial 
// which order of operations has precedence over * / % - + 
double after_primary(double x, Token& t) {
	double result;
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
		if (! assignment_chance || assignment_done)
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

// calculate of elements / % * (quotient. modulo quotient, product)
// which order of operations has precedence over - + 
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

// calculate of elements sums and differences
// which order of operations is the lowest
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

void reset_global_variables() {
	round_braces = false;   // set flag to false after error to cleaning before next operations
	square_braces = false;  // set flag to false after error to cleaning before next operations
	assignment_chance = false;
	assignment_done = false;
}

// validate of declaration
double declaration() {
	Token t = ts.get_token_after_SPACE();
	bool is_constant = ts.is_constant_token(t);
	if (exist(t.kind, NOT_VARIABLES_TOKENS)) {
		string keyword_name = get_reserved_name(t);
		error(keyword_name + " is keyword and can not be used as variable");
	}
	if (exist(t.name, NOT_VARIABLES_NAMES)) 
		error(t.name + " is keyword and can not be used as variable");
	if (t.kind != NAME) { 
		ts.unget(t);
		error("variable name expected in declaration",
			" name must started with letter or _ and may contain letters, digits and _ ");
	}
	Token t2 = ts.get_token_after_SPACE();
	if (t2.kind != '=') {
		ts.unget(t2);
		error("= missing in declaration of ", t.name);
	}
	double value = expression();
	symbols.define_name(t.name, value, is_constant);
	return value;
}

double statement() {
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
			double value = expression();
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
				manual.print_help();
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
		catch (Manual& m) {
			system("clear");
			manual.print_help();
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
	symbols.define_name("PI", 3.14159, true);
	symbols.define_name("e", 2.71828, true);
}

int main()
try {
	cout << "Welcome in simple calculator.\n";
	manual.print_help();
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
