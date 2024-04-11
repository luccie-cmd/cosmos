#if !defined(_APEX_PARSER_HH_)
#define _APEX_PARSER_HH_
#include "lexer.hh"
#include "token.hh"
#include "ast.hh"
#include <vector>

namespace apx{
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
        void expect(TokenType tt, std::string errormessage){
            if(current.get_type() != tt){
                fmt::print("{}", errormessage);
                exit(1);
            }
            this->consume();
        }
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
};
};

#endif // _APEX_PARSER_HH_
