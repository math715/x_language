//
// Created by ruoshui on 7/1/18.
//

#include <iostream>
#include <compile/node.h>

extern int yyparse();
extern BlockNode *program_block;

int main() {
    yyparse();
    std::cout << program_block << std::endl;
    return 0;
}