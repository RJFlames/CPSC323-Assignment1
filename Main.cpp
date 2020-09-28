#include <iostream>
#include <fstream>
#include <string>


int main(int argc, const char * argv[]) {
	std::ifstream source(argv[1]);
	std::ofstream out("LexerOutput.txt");
	// record latest;  
	//while (record.getToken() != "fileend"){
		//latest = callLexer(&source); 
		//out << latest.getToken() << \t:\t << latest.getLexeme();

		//latest = callLexer(source.get())
		//if (latest.isDone() == 1){		//out << latest.getToken() << \t:\t << latest.getLexeme(); } //
	//}

	out.close();
	source.close();
	return 0;
}

// Two versions | One where the lexer reads the file until a full token is found | and one where we feed the lexer one char at a time and only print when we find a full token
