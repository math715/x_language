//
// Created by ruoshui on 7/1/18.
//

#include <iostream>
#include <compile/node.h>
#include <compile/codegen.h>

extern int yyparse();
extern BlockNode *program_block;

void createCoreFunction(CodeGenContext& context);
int main() {
    yyparse();
    std::cout << program_block << std::endl;

    InitializeNativeTarget();
    InitializeNativeAsmPrinter();
    InitializeNativeAsmParser();
    CodeGenContext context;
    createCoreFunction(context);
    context.generateCode(program_block);
    context.runCode();
    return 0;
}