#include "ast.hh"
#include <fmt/core.h>
#include <assert.h>

namespace cosmos{
void AstNode::print(){
    switch(this->type){
    case AstType::VAR_ASSIGN: {
        Node.varAssign->print();
    } break;
    case AstType::VAR_DECL: {
        Node.varDecl->print();
    } break;
    case AstType::INCLUDE: {
        Node.include->print();
    } break;
    case AstType::STRING_LITERAL: {
        Node.strlit->print();
    } break;
    case AstType::USING: {
        Node.Nusing->print();
    } break;
    case AstType::EXPRESSION: {
        Node.expr->print();
    } break;
    case AstType::INTEGER_LITERAL: {
        Node.intlit->print();
    } break;
    case AstType::INVALID:
    default: {
        fmt::print("Invalid type: {}\n", static_cast<int>(this->type));
        exit(1);
    } break;
    }
}
void AstNode::setNode(AstTypes types){
    if (std::holds_alternative<NodeVarAssign*>(types)) {
        this->Node.varAssign = std::get<NodeVarAssign*>(types);
    } else if (std::holds_alternative<NodeVarDecl*>(types)) {
        this->Node.varDecl = std::get<NodeVarDecl*>(types);
    } else if (std::holds_alternative<NodeInclude*>(types)) {
        this->Node.include = std::get<NodeInclude*>(types);
    } else if (std::holds_alternative<NodeStringLit*>(types)) {
        this->Node.strlit = std::get<NodeStringLit*>(types);
    } else if (std::holds_alternative<NodeIntLit*>(types)) {
        this->Node.intlit = std::get<NodeIntLit*>(types);
    } else if (std::holds_alternative<NodeUsing*>(types)) {
        this->Node.Nusing = std::get<NodeUsing*>(types);
    } else if (std::holds_alternative<NodeExpr*>(types)) {
        this->Node.expr = std::get<NodeExpr*>(types);
    } else {
        fmt::print("Invalid AstTypes variant {}\n", types.index());
        exit(1);
    }
}
AstTypes AstNode::getNode(){
    switch(this->type){
    case AstType::VAR_ASSIGN: {
        return this->Node.varAssign;
    } break;
    case AstType::VAR_DECL: {
        return this->Node.varDecl;
    } break;
    case AstType::INVALID:
    default: {
        fmt::print("Invalid type: %d\n", static_cast<int>(this->type));
        exit(1);
    } break;
    }
}
void NodeInclude::print(){
    fmt::print("Include\n");
    fmt::print("|- Path: ");
    path->print();
}
void NodeVarDecl::print(){
    fmt::print("Var Decl\n");
    fmt::print("|- Name = {}\n", this->name);
    fmt::print("|- type = {}\n", (int)this->type);
    if(this->assign){
        assert(this->Nassign);
        this->Nassign->print();
    }
}
void NodeVarAssign::print(){
    fmt::print("Var Assign\n");
    fmt::print("|- Name = {}\n", this->name);
    fmt::print("|- Value = ");
    value->print();
}
void NodeStringLit::print(){
    fmt::print("String literal ");
    fmt::print("- Value = {}\n", this->value);
}
void NodeIntLit::print(){
    fmt::print("Integer literal ");
    fmt::print("- Value = {}\n", this->value);
}
void NodeUsing::print(){
    fmt::print("Using\n");
    fmt::print("|- Constructor type = {}\n", (int)this->type);
    fmt::print("|- Name = {}\n", this->name);
}
void NodeBinaryExpr::print(){
    fmt::print("    Binary\n");
    fmt::print("    |- Left: ");
    this->left->print();
    fmt::print("    |- Operator = {}\n", (int)this->type);
    fmt::print("    |- Right: ");
    this->right->print();
}
void NodeExpr::print(){
    fmt::print("Expression\n");
    switch(this->expr_type){
        case ExpressionType::BINARY: {
            this->Node.binary_expr->print();
        } break;
        case ExpressionType::INVALID:
        default: {
            fmt::print("Invalid expression type: {}\n", (int)this->expr_type);
            exit(1);
        } break;
    }
}
};