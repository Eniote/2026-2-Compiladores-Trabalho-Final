#include <fstream>
#include <iosfwd>
#include <iostream>

#include "config.h"

using namespace std;

struct token {
    int type;
    string* name;
};

extern int yylex();
extern char* yytext;
extern void yyrestart( FILE* new_file);

int main(int argc, char* argv[]) {

    yyrestart(fopen(INPUT_FILE_PREFIX, "r"));

    fstream fout(OUTPUT_FILE_PREFIX);

    cout << "starting lexer: \n";

    int out{};

    do {
        out = yylex();
        fout << out << ", " << yytext << endl;
    }while (out);


    fout.close();
    return 0;
}