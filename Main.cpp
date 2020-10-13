#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>
#include <iomanip>

const std::vector<std::string> keywords = {"int", "float", "return", "function", "while", "if", "fi", "put" , "Boolean" , "real" , "get" , "true" , "false"};
const std::vector<std::string> seps = { "[", "]", "(", ")", "{", "}", ";", ":"  , "$$" , ",", "$"};
const std::vector<std::string> ops = { "=", "!", "<", ">", "-", "+", "*", "/" , "<=", ">=", "!=", "-=", "+="};

class record {
private:
	std::string token,lexeme;
public:
	std::string getToken() { return this->token; }
	std::string getLexeme() { return this->lexeme; }

	void setLexeme(std::string s) { this->lexeme = s; }
	void setToken(std::string s) { this->token = s; }
};

bool FSM(std::string &state, char input, std::string &lexeme) {
	std::string c;
	c.push_back(input);
	if (state == "start") {
		if (isalpha(input)) { state = "identifier"; }
		else if (isdigit(input)) { state = "int"; }
		else if (input == '.') { state = "dot"; }
		else if (std::find(ops.begin(), ops.end(), c) != ops.end()) { state = "operator"; }
		else if (std::find(seps.begin(), seps.end(), c) != seps.end()) { state = "separator"; }
		else if (input == EOF) {
			state = "fileend";
			return true;
		}
	}

	else if (state == "identifier" && !isalnum(input) && input != '_') {
			return true;
		}
	else if (state == "int") {
		if (input == '.') { state = "dot"; }
		else if (!isdigit(input)) {
			return true;
		}
	}
	else if (state == "dot") {
		if (isdigit(input)) {
			state = "real";
		}
		else {
			state = "int";
			return true;
		}
	}
	else if (state == "real" && !isdigit(input)) {
		return true;
	}
	else if (state == "operator" && std::find(ops.begin(), ops.end(), lexeme + c) == ops.end()) {
		return true;
	}
	else if (state == "separator" && std::find(seps.begin(), seps.end(), lexeme + c) == seps.end()) {
		return true;
	}

	return false;
}

record callLexer(std::ifstream& source) {
	std::string state = "start", lexeme = "";
	int done = 0;
	char c;
	while (done != 1) {
		c = source.get();

		if (FSM(state, c, lexeme) == true) { 
			done = 1; 
			source.unget();
		}

		if (state != "comments" && lexeme == "/*") {
			state = "comments";
			lexeme = "";
		}
		else if (state == "comments" && c == '*' && (c = source.get()) == '/') {
			state = "start";
			c = source.get();
		}

		if (done == 1) {
			if (state == "identifier" && std::find(keywords.begin(), keywords.end(), lexeme) != keywords.end()) { state = "keyword"; }
			record latest;
			latest.setLexeme(lexeme);
			latest.setToken(state);
			return latest;
		}

		if (!isspace(c) && state != "comments") { lexeme.push_back(c); }
	}

}

int main(int argc, const char * argv[]) {

	std::ifstream source(argv[1]);
	std::ofstream out(argv[2]);

	record latest;  
	do {
		latest = callLexer(source); 
		if (latest.getToken() != "fileend") out << std::left << std::setw(10) << latest.getToken() << "\t:\t" << std::setw(10) << latest.getLexeme() << "\n";
	} while (latest.getToken() != "fileend");

	out.close();
	source.close();
	return 0;
}

