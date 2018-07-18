//
// Created by ruoshui on 7/17/18.
//

#include <iostream>
#include <vector>
#include "codegen.h"

void CodeGenContext::generateCode(BlockNode *root) {
    std::cout << "Generating code ..." << std::endl;
    std::vector<llvm::Type *> arg_types;
    llvm::FunctionType *ftype = llvm::FunctionType::get(llvm::Type::getVoidTy(MyContext), llvm::makeArrayRef(arg_types), false);
    main_function = llvm::Function::Create(ftype, llvm::GlobalValue::InternalLinkage, "main", module);
    llvm::BasicBlock *bblock = llvm::BasicBlock::Create(MyContext, "entry", main_function, 0);

    pushBlock(bblock);
    root->codeGen(*this);
    llvm::ReturnInst::Create(MyContext, bblock);
    popBlock();

    std::cout << "Code is generated.\n";

    llvm::PassManager pm;
    pm.add(createPrintModulePass(outs()));
    pm.run(*module);

}


llvm::GenericValue CodeGenContext::runCode() {
    std::cout << "Running code...\n";
}