//
// Created by ruoshui on 7/17/18.
//

#ifndef PROJECT_CODEGEN_H
#define PROJECT_CODEGEN_H


#include <stack>
#include <map>
#include <llvm/IR/Value.h>
#include <llvm/ExecutionEngine/GenericValue.h>


class BlockNode;

static llvm::LLVMContext MyContext;

class CodeGenBlock {
public:
    llvm::BasicBlock *block;
    llvm::Value *return_value;
    std::map<std::string, llvm::Value*> locals;
};

class CodeGenContext {
    std::stack<CodeGenBlock *> blocks;
    llvm::Function *main_function;

public:
    llvm::Module *module;
    CodeGenContext() {
        module = new llvm::Module("main");
    }

    void generateCode(BlockNode *root);
    llvm::GenericValue runCode();
    std::map<std::string, llvm::Value *> &locals() {
        return blocks.top()->locals;
    };
    llvm::BasicBlock *currentBlock() {
        return blocks.top()->block;
    }

    void pushBlock(llvm::BasicBlock *block) {
        blocks.push(new CodeGenBlock());
        blocks.top()->block = block;
        blocks.top()->return_value = nullptr;

    }
    void popBlock() {
        CodeGenBlock *top = blocks.top();
        blocks.pop();
        delete top;
    }
    void setCurrentReturnValue(llvm::Value *value ){
        blocks.top()->return_value = value;
    }
    llvm::Value* getCurrentReturnValue() {
        return blocks.top()->return_value;
    }
};
#endif //PROJECT_CODEGEN_H
