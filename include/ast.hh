#if !defined(_COSMOS_AST_HH_)
#define _COSMOS_AST_HH_
#include <string>
#include <variant>
#include <cstdint>
#include <vector>

namespace cosmos{
class AstNode;
enum struct AstType{
    INVALID,
    VAR_DECL,
    VAR_ASSIGN,
    VAR_ACCESS,
    INCLUDE,
    USING,
    NAMESPACE_DECL,
    FUNC_DECL,
    EXPRESSION,
    COMPOUND,
    INTEGER_LITERAL,
    STRING_LITERAL,
    TYPE,
    ASSIGNOP,
    CONSTRUCTOR, // This is something like a `namespace` `class` or `struct` (don't ask me why) not to be confused with a constructor for a class
};
enum struct ExpressionType{
    INVALID,
    BINARY,
    CALL,
};
enum struct AssignOp{
    INVALID,
    SHREQUAL,
    SHLEQUAL,
    PLUSEQUAL,
    MINUSEQUAL,
    TIMESEQUAL,
    DIVIDEEQUAL,
    EQUAL,
};
enum struct Types{
    INVALID,
    INT,
    STRING,
};
enum struct Operator{
    INVALID,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MOD,
};
// TODO: Support more constructor types like classes and structs
enum struct ConstructorType{
    INVALID,
    NAMESPACE,
};
class NodeBinaryExpr{
    private:
        Operator type;
        AstNode *left;
        AstNode *right;
    public:
        NodeBinaryExpr(AstNode* left, Operator type, AstNode* right){
            this->left = left;
            this->right = right;
            this->type = type;
        }
        void print();
};
class NodeExpr{
    private:
        ExpressionType expr_type;
        union{
            NodeBinaryExpr *binary_expr;
        } Node;
    public:
        NodeExpr(ExpressionType expr_type){
            this->expr_type = expr_type;
        }
        ExpressionType get_expr_type() { return expr_type; }
        void set_binary_expr(NodeBinaryExpr* binary_expr) { Node.binary_expr = binary_expr; }
        void print();
};
// This one is for defining something like `namespace NAME{};` and not `using namespace NAME;`
class NodeContructor{
    private:
        ConstructorType type;
        std::string name;
        bool define;
        std::vector<AstNode*> children;
    public:
        NodeContructor(ConstructorType type, std::string name, bool define){
            this->type = type;
            this->name = name;
            this->define = define;
        }
        ConstructorType get_type() { return type; }
        std::string get_name() { return name; }
        std::vector<AstNode*> get_children() { return children; }
        bool get_define() { return define; }
        
        void add_child(AstNode* child) { children.push_back(child); }
        void print();
};
// +=, -=, *=, /=, %=, &=, |=, ^=, <<=, >>= are examples of things put inside the assign op
class NodeAssignOp{
    private:
        AssignOp op;
    public:
        NodeAssignOp(AssignOp op){
            this->op = op;
        }
        AssignOp get_op() { return op; }
        void print();
};
class NodeUsing {
    private:
        std::string name;
        ConstructorType type;
    public:
        NodeUsing(std::string name, ConstructorType type){
            this->name = name;
            this->type = type;
        }
        std::string get_name() { return name; }
        ConstructorType get_type() { return type; }
        void print();
};
class NodeInclude{
    private:
        AstNode* path;
    public:
        NodeInclude(AstNode* path){
            this->path = path;
        }
        AstNode* get_path() { return path; }
        void set_path(AstNode* path) {this->path = path;}
        void print();
};
class NodeVarAssign{
    private:
        std::string name;
        NodeAssignOp* assignop;
        AstNode* value;
    public:
        NodeVarAssign(std::string name, NodeAssignOp* assignop, AstNode* value){
            this->name = name;
            this->assignop = assignop;
            this->value = value;
        }
        // NodeVarAssign(std::string& name, NodeAssignOp*& assignop, AstNode*& value){
        //     this->name = std::string(name.c_str(), name.size());
        // //     this->assignop = assignop;
        // //     this->value = value;
        // }
        std::string get_name() { return name; }
        AstNode* get_value() { return value; }
        void print();
};
class NodeVarDecl{
    private:
        std::string name;
        Types type;
        bool assign;
        NodeVarAssign *Nassign;
    public:
        NodeVarDecl(std::string name, Types type, bool assign, NodeVarAssign* Nassign){
            this->name = name;
            this->type = type;
            this->assign = assign;
            this->Nassign = Nassign;
        }
        std::string get_name() { return name; }
        Types get_type() { return type; }
        bool get_assign() { return assign; }
        NodeVarAssign* get_Nassign() { return Nassign; }
        void set_Nassign(NodeVarAssign* Nassign) { this->Nassign = Nassign; }
        void print();
};
class NodeStringLit{
    private:
        std::string value;
    public:
        NodeStringLit(std::string value){
            this->value = value;
        }
        NodeStringLit(const char* c_str){
            this->value = std::string(c_str);
        }
        std::string get_value() { return this->value; }
        void print();
};
class NodeIntLit{
    private:
        uint64_t value;
    public:
        NodeIntLit(uint64_t value){
            this->value = value;
        }
        uint64_t get_value() { return this->value; }
        void print();
};

// All of the possible AstTypes in a single 'variable'
using AstTypes = std::variant<NodeVarAssign*, NodeVarDecl*, NodeInclude*, NodeStringLit*, NodeIntLit*, NodeUsing*, NodeExpr*>;
class AstNode{
    private:
        AstType type;
        union Node{
            NodeVarDecl* varDecl;
            NodeVarAssign* varAssign;
            NodeInclude* include;
            NodeStringLit* strlit;
            NodeIntLit* intlit;
            NodeUsing* Nusing;
            NodeExpr* expr;
            Node() noexcept {}
            ~Node() noexcept {}
        } Node;
    public:
        AstNode(AstType type=AstType::INVALID){
            this->type = type;
        }
        AstType get_type() { return this->type; }
        void set_type(AstType type) { this->type = type; }
        void print();
        void setNode(AstTypes types);
        AstTypes getNode();
};
};

#endif // _COSMOS_AST_HH_
