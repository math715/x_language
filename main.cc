//
// Created by ruoshui on 7/1/18.
//

#include <iostream>
#include <compile/node.h>
#include <compile/codegen.h>

extern int yyparse();
extern FILE *yyin;
extern BlockNode *program_block;

extern void createCoreFunctions(CodeGenContext& context);

int main(int argc, char *argv[]) {
    yyin = fopen(argv[1], "r");
    if (yyin == nullptr) {
        std::cerr << "Could not find file" << argv[1] << std::endl;
        return 1;
    }
    int ret = yyparse();
    std::cout << ret << std::endl;
//    if (yyparse() != 0){
//        return 1;
//    }
    if (ret != 0 ) {
        return 1;
    }
    std::cout << program_block << std::endl;
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
    CodeGenContext context;
    createCoreFunctions(context);
    context.generateCode(*program_block);
    context.runCode();
    return 0;
}