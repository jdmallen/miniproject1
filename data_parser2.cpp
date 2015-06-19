#include <iostream>
#include <stack>
#include <set>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <locale>
#include <cstring>
#include "Lexer.h"

using namespace std;

void printTokens(Token t){
    cout << t.type << " " << t.value << endl;
}

int main(int argc, char *argv[]){
    
    string inFileName, outFileName;
    Lexer lexer;
    Token tok;
    string line;
    locale loc;
    
    if (argc == 1){
        cout << "Please provide an input filename." << endl;
        cin >> inFileName;
        cout << "Please provide an output filename." << endl;
        cin >> outFileName;
    } else if (argc == 2){
        cerr << "Usage: ./data_parser <inputfile> <outputfile>" << endl;
        return 1;
    } else {
        inFileName = argv[1];
        outFileName = argv[2];
    }

    ifstream input (inFileName.c_str());
    ofstream output;
    if(!input.is_open()){
        cerr << "Cannot open input file." << endl;
        return 1;
    } else {
        output.open(outFileName.c_str());
        stringstream buffer;
        buffer << input.rdbuf();
        string fileContents(buffer.str());    

        lexer.set_input(fileContents);
        vector<Token> tokens = lexer.tokenize();
        
        for (int i = 0; i < tokens.size(); i++){
            tok = tokens[i];
            switch (tok.type) {
                case IDENT:    cout << "IDENT:   " << tok.value << endl; break;
                case TAG:      cout << "TAG:     " << tok.value << endl; break;
                case ENDTOK:   cout << "EOF:     " << endl; break;
                case ERRTOK:   cout << "[ ERROR ]" << endl; break;
            }
        }

      /*locale loc;
        set<string> tags;
        string unwantedChar = "/", value;
        for (int i = 0; i < tokens.size(); i++){
            tok = tokens[i];
            value = tok.value;
            if (tok.type == TAG && value.size() < 20 && 
                value.compare(0,1,unwantedChar) != 0){
                char *cval = new char[value.size() + 1];
                strcpy(cval, value.c_str());
                for (char i = 0; i < value.size() + 1; ++i){
                    char c = cval[i];
                    if (c == 95){
                        cval[i] = 32;
                    } else {
                        cval[i] = toupper(c);
                    }
                }
                string newVal = cval;
                tags.insert(newVal);
            }
        }
        
        set<string>::iterator it;
        for (it = tags.begin(); it != tags.end(); ++it){
             output << *it << endl;
        }*/
        
        
        output.close();
    }

    return 0;
}   

