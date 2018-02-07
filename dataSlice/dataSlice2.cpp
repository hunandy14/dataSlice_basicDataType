#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <cstdlib>
#include <limits>
#include <cerrno>
#include <regex>

using namespace std;

class DefList{
public:
	int Definition_Count;
	vector<string> DefSymbolList;
	vector<int> Definition_Address;
};

class ProgramText{
public:
	int ProgramText_Count;
	vector<string> IARE_List;
	vector<string> Instruction_Address;
};




/* Containers */
// to store all line;
vector<string> inputLine;

// to store all token
vector<string> tokenVector;
// to record positions of each token.
vector<int> rowVector; // to store the number of row on each token
vector<int> columnVector; // to store the number of vector on each token
						  //
map<string, int> FreqSymDef; // frequency of defining a symbol <symbol token , frequency>
map<string, int> Count_Sym_Module; // log the position where the symbol is <symbol token , modulePos>
map<string, int> DictSymAddress; // Dictionary for {Symbol : Address}



// Index Area
int token_index = 0; // token index when parsing
int num_token; // numbers of token in the file

/* Read File and Parse it */
void parseFile(const char* argv){
	// Stream the input file
	string text;
	ifstream inputFile(argv, ios::binary);
	if(inputFile.is_open()){
		text.assign((std::istreambuf_iterator<char>(inputFile)), (std::istreambuf_iterator<char>()));

		vector<int> lengthLine; // log the length of each line


								/* Parse the token */
								/* Log the position (row , column) */
		int row = 0; // Initialization
		int column = 0;

		string token = "";
		for(int i = 0; i < text.size(); i++){
			row += 1;

			if(text[i] != ' ' || text[i] != '\t' || text[i] != '\n'){

				if(text[i + 1] == ' ' || text[i + 1] == '\t' || text[i + 1] == '\n'){
					tokenVector.push_back(token);
					token = "";
				}
				column += 1;
				token += text[i];
			} else if(text[i] == ' '){
				column += 1;
				token = "";
			} else if(text[i] == '\t'){
				column += 1;
				token = "";
			} else if(text[i] == '\n'){
				column = 0;
				token = "";
			}
		}
		for(int i = 0; i < tokenVector.size(); i++){
			cout << tokenVector[i] << endl;
		}

		inputFile.close();
	} else{
		cout << "Failed to open the file." << endl;
	}
}


/* Main Function */
int dataSlice2(const char* argv){
	parseFile(argv);
	return 0;
}