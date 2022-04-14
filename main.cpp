
#include <iostream>
#include "scanner.h"
#include "token.h"
#include "parser.h"
#include "scheme.h"
#include "tuple.h"
#include "relation.h"
#include "interpreter.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (!argv[1]) {
        cout << "Must include file name in commandline" << endl;
        cout << "Total Tokens = 0" << endl;
    }
    else {
        scanner myScanner;
        myScanner.scan(argv[1]);
        //ofstream out(argv[2]);
        parser myParser(myScanner.getTokenVector());
        myParser.parseTokens();
        interpreter myInterpreter(myParser.getDatalog());
    }
   

    return 0;
}
