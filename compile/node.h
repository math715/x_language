//
// Created by ruoshui on 6/19/18.
//

#ifndef PROJECT_NODE_H
#define PROJECT_NODE_H

#include <llvm/IR/Value.h>
#include <vector>
#include <string>

class CodeGenContext;

class Statement;
class Expression;
class VariableDeclaration;

using StatementList = std::vector<Statement *> ;
using ExpressionList = std::vector<Expression *> ;
using VariableList = std::vector<VariableDeclaration*>;

class Node {
public:
    virtual  ~Node() {};
    virtual llvm::Value* codeGen(CodeGenContext &context) {
        return nullptr;
    }
};

class Expression : public Node {

};

class Statement : public Node {

};


class BooleanNode : public Expression {
public:
    bool value_;
    BooleanNode(bool value) : value_(value){}
    virtual llvm::Value* codeGen(CodeGenContext &context);
};
class IntegerNode : public Expression {
public :
    int value_;
    IntegerNode(int value) : value_(value){}
    virtual llvm::Value* codeGen(CodeGenContext &context);
};

class DoubleNode : public Expression {
public:
    double value_;
    DoubleNode(double value) : value_(value){}
    virtual llvm::Value* codeGen(CodeGenContext &context);
};

class StringNode : public Expression{
public:
    std::string value_;
    StringNode(const std::string &value): value_(value) {}
    virtual  llvm::Value * codeGen(CodeGenContext &contxt);
};

class IdentifierNode : public Expression {
public:
    std::string value_;
    IdentifierNode(const std::string &value): value_(value){}
    virtual llvm::Value *codeGen(CodeGenContext &context);
};

class FunctionCallNode : public Expression {
public :
    const IdentifierNode &id_;
    ExpressionList  arguments_;
    FunctionCallNode(const IdentifierNode &id, ExpressionList &arguments):
            id_(id), arguments_(arguments){

    }
    FunctionCallNode(const IdentifierNode &id): id_(id) {}
    virtual llvm::Value* codeGen(CodeGenContext &context);

};

class BinaryOperatorNode: public Expression {
public:
    Expression &lhs_;
    int op_;
    Expression &rhs_;
    BinaryOperatorNode(Expression &lhs, int op, Expression &rhs)
            : lhs_(lhs), op_(op), rhs_(rhs) {

    }

    virtual llvm::Value *codeGen(CodeGenContext &context);

};

class AssignmentNode: public Expression {
public:
    IdentifierNode &lhs_;
    Expression & rhs_;
    AssignmentNode(IdentifierNode &lhs, Expression &rhs): lhs_(lhs), rhs_(rhs){}
    virtual llvm::Value * codeGen(CodeGenContext &context);
};

class BlockNode : public Expression {
public:
    StatementList  statements_;
//    BlockNode(StatementList &statments) : statements_(statments){}
    BlockNode() {}
    virtual llvm::Value * codeGen(CodeGenContext &context);
};

class ExpressionStatement : public Statement {
public:
    Expression &expression_;
    ExpressionStatement(Expression &expression) : expression_(expression){}
    virtual llvm::Value *codeGen(CodeGenContext &context);
};

class ReturnStatement : public Statement {
public:
    Expression &expression_;
    ReturnStatement(Expression &expression) :expression_(expression){}
    virtual llvm::Value *codeGen(CodeGenContext &context);

};

class VariableDeclaration : public Statement {
public:
    const IdentifierNode &type_;
    IdentifierNode &id_;
    Expression *assignment_expr_;
    VariableDeclaration( const IdentifierNode &type, IdentifierNode &id)
            : type_(type), id_(id), assignment_expr_(nullptr){

    }
    VariableDeclaration ( const IdentifierNode &type, IdentifierNode &id, Expression *assignment_expr)
            : type_(type), id_(id), assignment_expr_(assignment_expr){

    }
    virtual llvm::Value* codeGen(CodeGenContext &context);
};

class ExternDecleration : public Statement {
public:
    const IdentifierNode &type_;
    IdentifierNode &id_;
    VariableList arguments_;
    ExternDecleration(const IdentifierNode &type, IdentifierNode &id, const VariableList& arguments)
            : type_(type), id_(id), arguments_(arguments){}
    virtual llvm::Value *codeGen(CodeGenContext &context);
};

class FunctionDeclaration: public  Statement {
public:
    const IdentifierNode &type_;
    IdentifierNode &id_;
    VariableList  arguments_;
    BlockNode &blocks_;
    FunctionDeclaration( const IdentifierNode &type, IdentifierNode &id, VariableList &arguments, BlockNode &blocks)
            : type_(type), id_(id), arguments_(arguments), blocks_(blocks){

    }
    virtual  llvm::Value *codeGen(CodeGenContext &context);
};


#endif //PROJECT_NODE_H
