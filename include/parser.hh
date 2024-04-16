#if !defined(_COSMOS_PARSER_HH_)
#define _COSMOS_PARSER_HH_
#include "lexer.hh"
#include "token.hh"
#include "ast.hh"
#include <vector>

namespace cosmos{
class Parser{
    private:
        std::vector<Token> tokens;
        size_t idx;
        Token current;
        void consume(){
            try{
                this->current = this->tokens.at(idx++);
            } catch(const std::out_of_range& err){
                (void)err;
                this->current = Token("EOF", TokenType::TEOF);
            }
        }
        Token expect(TokenType tt, std::string errormessage){
            if(current.get_type() != tt){
                fmt::print("{}", errormessage);
                exit(1);
            }
            Token t = current;
            this->consume();
            return t;
        }
        AstNode* parse_binary_expr();
        AstNode* parse_add_binary_expr(); // Handles + and - 
        AstNode* parse_mul_binary_expr(); // Handles * and /
    public:
        Parser(Lexer lexer) :current(Token("Invalid", TokenType::INVALID)) {
            this->tokens = lexer.lex_all_tokens();
            this->idx = 0;
            this->consume();
        }
        Parser(std::string file_data) :current(Token("Invalid", TokenType::INVALID)) {
            Lexer lexer(file_data);
            this->tokens = lexer.lex_all_tokens();
            this->idx = 0;
            this->consume();
        }
        AstNode* preprocessor();
        AstNode* next_node();
        AstNode* parse_using();
        AstNode* parse_var();
        AstNode* parse_expr();
        AstNode* parse_string();
        AstNode* parse_primary_expr();
        NodeAssignOp* parse_assign_op();
        Types parse_type();
        ConstructorType parse_constructor_type();
        Operator parse_operator();
};
};

#endif // _COSMOS_PARSER_HH_
