#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>

const std::vector<std::string> keywords = {"int", "float", "return", "function", "while", "if", "fi"};
const std::vector<char> seps = { '[', ']', '(', ')', '{', '}', ';', ':'  ,'$', ',' };
const std::vector<char> ops = { '=', '!', '<', '>', '-', '+', '*', '/'};

class record {
private:
	std::string token,lexeme;
public:
	std::string getToken() { return this->token; }
	std::string getLexeme() { return this->lexeme; }

	void setLexeme(std::string s) { this->lexeme = s; }
	void setToken(std::string s) { this->token = s; }
};

record callLexer(std::ifstream& source) {
	std::string state = "start", lexeme = "";
	int done = 0;
	char c;
	while (done != 1) {
		c = source.get();
		if (state == "start") {
			if (isalpha(c)) { state = "identifier"; }
			else if (isdigit(c)) { state = "int"; }
			else if (std::find(ops.begin(), ops.end(), c) != ops.end()) { state = "operator"; }
			else if (std::find(seps.begin(), seps.end(), c) != seps.end()) { state = "separator";}
			else if (c == EOF) {
				state = "fileend";
				done = 1;
			}
		}
		else if (state == "identifier" && !isalnum(c)) { done = 1; }
		else if (state == "int") {
			if (c == '.') { state == "real"; }
			else if (!isdigit(c)) {
				done = 1;
				source.unget();
			}
		}
		else if (state == "real" && !isdigit(c)) {
			done = 1;
			source.unget();
		}
		else if (state == "operator" && !ispunct(c)) { done = 1; }
		else if (state == "separator" && !ispunct(c)) { done = 1; }

		if (done == 1) {
			if (state == "id" && std::find(keywords.begin(), keywords.end(), lexeme) != keywords.end()) { state = "keyword"; }
			record latest;
			latest.setLexeme(lexeme);
			latest.setToken(state);
		}

		lexeme.push_back(c);
	}

}

int main(int argc, const char * argv[]) {
	std::ifstream source(argv[1]);
	std::ofstream out("LexerOutput.txt");
	record latest;  
	while (latest.getToken() != "fileend"){
		latest = callLexer(source); 
		out << latest.getToken() << "\t:\t" << latest.getLexeme();

	}

	out.close();
	source.close();
	return 0;
}

