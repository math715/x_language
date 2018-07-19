//
// Created by ruoshui on 7/19/18.
//

#include <iostream>
#include "codegen.h"
#include "node.h"

extern int yyparse();
extern BlockNode *program_block;

llvm::Function *createPrintFunction(CodeGenContext &context) {
    std::vector<llvm::Type *> printf_arg_types;
    printf_arg_types.push_back(llvm::Type::getInt8PtrTy(MyContext));

    llvm::FunctionType *printf_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(MyContext), printf_arg_types, true);
    llvm::Function *func = llvm::Function::Create(printf_type, llvm::Function::ExternalLinkage, llvm::Twine("printf"), context.module);

    func->setCallingConv(llvm::CallingConv::C);
    return func;
}

void CreateEchoFunction(CodeGenContext &context, llvm::Function *printfFn) {
    std::vector<llvm::Type *> echo_arg_types;
    echo_arg_types.push_back(llvm::Type::getInt64Ty(MyContext));
    llvm::FunctionType *echo_type = llvm::FunctionType::get( llvm::Type::getVoidTy(MyContext), echo_arg_types, false);
    llvm::Function *func = llvm::Function::Create(echo_type, llvm::Function::InternalLinkage, llvm::Twine("echo"), context.module);

    llvm::BasicBlock *bblock = llvm::BasicBlock::Create(MyContext, "entry", func, 0);
    context.pushBlock(bblock);
    const char *constValue = "%d\n";
    llvm::Constant *format_const = llvm::ConstantDataArray::getString(MyContext, constValue);
    llvm::GlobalVariable *var = new llvm::GlobalVariable(*context.module, llvm::ArrayType::get(
            llvm::IntegerType::get(MyContext, 8), strlen(constValue) + 1), true, llvm::GlobalValue::PrivateLinkage, format_const, ".str");

    llvm::Constant *zero = llvm::Constant::getNullValue(llvm::IntegerType::getInt32Ty(MyContext));
    std::vector<llvm::Constant *> indices;
    indices.push_back(zero);
    indices.push_back(zero);

    llvm::Constant *var_ref = llvm::ConstantExpr::getGetElementPtr(
            llvm::ArrayType::get(llvm::IntegerType::get(MyContext, 8), strlen(constValue) + 1),
            var, indices            );
    std::vector<llvm::Value *> args;
    args.push_back(var_ref);
    llvm::Function::arg_iterator args_values = func->arg_begin();
    llvm::Value *toPrint = &*args_values++;
    toPrint->setName("toPrint");
    args.push_back(toPrint);

    llvm::CallInst *call = llvm::CallInst::Create(printfFn, llvm::makeArrayRef(args), "" , bblock);
    llvm::ReturnInst::Create(MyContext, bblock);
    context.popBlock();
}

void createCoreFunctions(CodeGenContext & context) {
    llvm::Function * printFn = createPrintFunction(context);
    CreateEchoFunction(context, printFn);
}