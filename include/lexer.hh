#if !defined(_COSMOS_LEXER_HH_)
#define _COSMOS_LEXER_HH_
#include <string>
#include <vector>
#include <cstdint>
#include <fmt/core.h>
#include "token.hh"

namespace cosmos{
class Lexer{
    private:
        std::string data;
        std::uint64_t curr;
        char c;
        inline bool at_eof(){
            return (this->c == '\0');
        }
        inline void next_char(){
            // if we can't increment count anymore make the current character NULL
            try{
                this->c = this->data.at(this->curr++);
            } catch(const std::out_of_range& err){
                (void)err;
                this->c = '\0';
            }
        }
    public:
        Lexer(std::string data){
            this->data = data;
            this->curr = 0;
            this->c = this->data.at(this->curr++);
        }
        void remove_whitespace();
        Token next_token();
        Token lex_identifier_or_keyword();
        Token lex_string();
        Token lex_number();
        std::vector<Token> lex_all_tokens();
};
};

#endif // _COSMOS_LEXER_HH_
