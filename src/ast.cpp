#include "ast.hh"
#include <fmt/core.h>
#include <assert.h>

namespace apx{
void AstNode::print(){
    switch(this->type){
    case AstType::VAR_ASSIGN: {
        Node.varAssign->print();
    } break;
    case AstType::VAR_DECL: {
        Node.varDecl->print();
    } break;
    case AstType::VAR_DECL_ASSIGN: {
        Node.varDeclAssign->print();
    } break;
    case AstType::INCLUDE: {
        Node.include->print();
    } break;
    case AstType::STRING_LITERAL: {
        Node.strlit->print();
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
    } else if (std::holds_alternative<NodeVarDeclAssign*>(types)) {
        this->Node.varDeclAssign = std::get<NodeVarDeclAssign*>(types);
    } else if (std::holds_alternative<NodeInclude*>(types)) {
        this->Node.include = std::get<NodeInclude*>(types);
    } else if (std::holds_alternative<NodeStringLit*>(types)) {
        this->Node.strlit = std::get<NodeStringLit*>(types);
    } else {
        fmt::print("Invalid AstTypes variant\n");
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
    case AstType::VAR_DECL_ASSIGN: {
        return this->Node.varDeclAssign;
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
    fmt::print("|- type = \n");
    type->print();
}
void NodeVarAssign::print(){
    fmt::print("Var Assign\n");
    fmt::print("|- Name = {}\n", this->name);
    fmt::print("|- Value = \n");
    value->print();
}
void NodeVarDeclAssign::print(){
    fmt::print("Var Decl assignment\n");
    fmt::print("|- Name = {}\n", this->name);
    fmt::print("|- type = \n");
    type->print();
    fmt::print("|- value = \n");
    value->print();
}
void NodeStringLit::print(){
    fmt::print("String literal ");
    fmt::print("- Value = {}\n", this->value);
}
};