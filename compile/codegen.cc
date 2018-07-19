//
// Created by ruoshui on 7/17/18.
//

#include <iostream>
#include <vector>
#include "codegen.h"
//#include "parser.h"

#include <llvm/IR/Constant.h>
#include "node.h"

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
    llvm::ExecutionEngine *ee = llvm::ExecutionEngine( std::unique_ptr<llvm::Module> ).create();
    ee->finalizeObject();
    std::vector<llvm::GenericValue> noargs;
    llvm::GenericValue v = ee->runFunction(main_function, noargs);
    std::cout << "Code was run.\n";
    return v;
}


static llvm::Type *typeOf (const IdentifierNode &type) {
    if (type.value_.compare("int") == 0) {
        return llvm::Type::getInt64Ty(MyContext);
    } else if (type.value_.compare("double") == 0) {
        return llvm::Type::getDoubleTy(MyContext);
    } else if (type.value_.compare("bool") == 0) {
        return llvm::Type::getBooleanTy(MyContext);
    }
    return llvm::Type::getVoidTy(MyContext);
}

llvm::Value* BooleanNode::codeGen(CodeGenContext &context) {
    std::cout << "Create Boolean: " << value_ << std::endl;
    return llvm::ConstantInt::get(llvm::Type::getInt64Ty(MyContext), value_, true);
}

llvm::Value* IntegerNode::codeGen(CodeGenContext &context) {
    std::cout << "Create Integer: " << value_ << std::endl;
    return llvm::ConstantInt::get(llvm::Type::getInt64Ty(MyContext), value_, true);
}

llvm::Value* DoubleNode::codeGen(CodeGenContext &context) {
    std::cout << "Create Integer: " << value_ << std::endl;
    return llvm::ConstantFP::get(llvm::Type::getDoubleTy(MyContext), value_);
}

llvm::Value* IdentifierNode::codeGen(CodeGenContext &context) {
    std::cout << "Create identifier reference: " << value_ << std::endl;
    if (context.locals().find(value_) == context.locals().end()){
        std::cerr << "undeclared variable " << value_ << std::endl;
        return nullptr;
    }
    return new llvm::Loadinst(context.locals()[value_], "", false, context.currentBlock());
}

llvm::Value* FunctionCallNode::codeGen(CodeGenContext &context) {
    llvm::Function *function = context.module->getFunction(id_.value_.c_str());
    if (function == nullptr) {
        std::cerr << "no such function " << id_.value_ << std::endl;
    }
    std::vector<llvm::Value*> args;
    for (auto& it : arguments_) {
        args.push_back((**it).codeGen(context));
    }
    llvm::CallInst *call = llvm::CallInst::Create(function, llvm::makeArrayRef(args), "", context.currentBlock());
    std::cout << "Creating method call:" << id_.value_ << std::endl;
    return call;
}

llvm::Value* BinaryOperatorNode::codeGen(CodeGenContext &context) {
    std::cout << "Creating binary operation.\n";
    llvm::Instruction::BinaryOps instr;
    switch (op_) {
        case ADD : instr = llvm::Instruction::Add; break;
        case MIS : instr = llvm::Instruction::Sub; break;
        case MUL : instr = llvm::Instruction::Mul; break;
        case DIV : instr = llvm::Instruction::SDiv; break;
    }
    return llvm::BinaryOperator::Create(instr, lhs_.codeGen(context>), rhs_.codeGen(context), "", context.currentBlock());
}

llvm::Value* AssignmentNode::codeGen(CodeGenContext &context) {
    std::cout << "Creating assignment for " << lhs_.value_ << std::endl;
    if (context.locals().find((lhs_.value_)) == context.locals().end()) {
        std::cerr << "undeclared variable " << lhs_.value_ << std::endl;
        return NULL;
    }
    return new llvm::StoreInst(rhs_.codeGen(context), context.locals()[lhs_.value_], false, context.currentBlock());
}

llvm::Value* BlockNode::codeGen(CodeGenContext &context) {
    llvm::Value *last = nullptr;
    for (auto &it : statements_) {
        std::cout << "Generating code for ..." << typeid(*it).name() << std::endl;
        last = (**it).codeGen(context);
    }
    std::cout << "Creating block\n";
    return last;

}

llvm::Value* ExpressionStatement::codeGen(CodeGenContext &context) {
    std::cout << "Generating code for " << typeid(expression_).name() << std::endl;
    return expression_.codeGen(context);
}



llvm::Value* VariableDeclaration::codeGen(CodeGenContext& context)
{
    std::cout << "Creating variable declaration " << type_.value_ << " " << id_.value_ << std::endl;
    llvm::AllocaInst *alloc = new AllocaInst(typeOf(type_), id_.value_.c_str(), context.currentBlock());
    context.locals()[id_.value_] = alloc;
    if (assignment_expr_ != NULL) {
        AssignmentNode assn(id_, *assignment_expr_);
        assn.codeGen(context);
    }
    return alloc;
}

llvm::Value* ExternDecleration::codeGen(CodeGenContext &context) {
    std::vector<llvm::Type*> argTypes;
    VariableList::const_iterator it;
    for (it = arguments_.begin(); it != arguments_.end(); it++) {
        argTypes.push_back(typeOf((**it).type));
    }
    llvm::FunctionType *ftype = llvm::FunctionType::get(typeOf(type_), makeArrayRef(argTypes), false);
    llvm::Function *function = llvm::Function::Create(ftype, llvm::GlobalValue::ExternalLinkage, id_.name.c_str(), context.module);
    return function;
}

llvm::Value* ReturnStatement::codeGen(CodeGenContext &context) {
    std::cout << "Generating return code for " << typeid(expression_).name() << std::endl;
    llvm::Value *return_value = expression_.codeGen(context);
    context.setCurrentReturnValue(return_value);
    return return_value;
}

llvm::Value* FunctionDeclaration::codeGen(CodeGenContext &context) {
    std::vector<llvm::Type *> argTypes;
    for (auto &it : arguments_) {
        argTypes.push_back(typeOf((**it).type));
    }
    llvm::FunctionType *ftype = llvm::FunctionType::get(typeOf(type), llvm::makeArrayRef(argTypes), false);
    llvm::Function *function = llvm::Function::Create(ftype, llvm::GlobalValue::InternalLinkage, id_.value_.c_str(), context.module);
    llvm::BasicBlock *bblock = llvm::BasicBlock::Create(MyContext, "entry", function, 0);
    context.pushBlock(bblock);
    llvm::Function::arg_iterator argsValues = function->arg_begin();
    llvm::Value *argument_value;
    for (auto &it : arguments_) {
        (**it).codeGen(context);
        argument_value = &*argsValues++;
        argument_value->setName((*it)->id_.value.c_str());
        llvm::StoreInst *inst = new llvm::StoreInst(argument_value, context, context.locals()[(*it)->id_.value_], false, bblock);

    }

    blocks_.codeGen(context);
    llvm::ReturnInst::Create(MyContext, context.getCurrentReturnValue(), bblock);
    context.popBlock();
    std::cout << "Creating function..." << id_.value_ << std::endl;
    return function;
}