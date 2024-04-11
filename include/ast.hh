#if !defined(_APEX_AST_HH_)
#define _APEX_AST_HH_
#include <string>
#include <variant>

namespace apx{
class AstNode;
enum struct AstType{
    INVALID,
    VAR_DECL_ASSIGN,
    VAR_DECL,
    VAR_ASSIGN,
    INCLUDE,
    USING,
    NAMESPACE_DECL,
    FUNC_DECL,
    STATEMENT,
    EXPRESSION,
    COMPOUND,
    INTEGER_LITERAL,
    STRING_LITERAL,
    TYPE,
};
enum struct StatementType{
    CALL,
};
enum struct ExpressionType{
    BINARY,
};
enum struct Types{
    INT,
    STRING,
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
        AstNode* value;
    public:
        NodeVarAssign(std::string name, AstNode* value){
            this->name = name;
            this->value = value;
        }
        std::string get_name() { return name; }
        AstNode* get_value() { return value; }
        void print();
};
class NodeVarDecl{
    private:
        std::string name;
        AstNode* type;
    public:
        NodeVarDecl(std::string name, AstNode* type){
            this->name = name;
            this->type = type;
        }
        std::string get_name() { return name; }
        AstNode* get_type() { return type; }
        void print();
};
class NodeVarDeclAssign{
    private:
        std::string name;
        AstNode* type;
        AstNode* value;
    public:
        NodeVarDeclAssign(std::string name, AstNode* type){
            this->name = name;
            this->type = type;
        }
        std::string get_name() { return name; }
        AstNode* get_type() { return type; }
        AstNode* get_value() { return value; }
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

// All of the possible AstTypes in a single 'variable'
using AstTypes = std::variant<NodeVarAssign*, NodeVarDecl*, NodeVarDeclAssign*, NodeInclude*, NodeStringLit*>;
class AstNode{
    private:
        AstType type;
        union Node{
            NodeVarDecl* varDecl;
            NodeVarAssign* varAssign;
            NodeVarDeclAssign* varDeclAssign;
            NodeInclude* include;
            NodeStringLit* strlit;
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

#endif // _APEX_AST_HH_
