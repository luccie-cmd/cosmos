#if !defined(_APEX_TOKEN_HH_)
#define _APEX_TOKEN_HH_
#include <string>

namespace apx{
enum struct TokenType{
    INVALID=0,
    TEOF=1, // Why not just EOF well C++ doesn't like that

    OPENPAREN = '(',
    CLOSEPAREN = ')',
    OPENBRACE = '{',
    CLOSEBRACE = '}',
    SEMICOLON = ';',
    COLON = ':',
    AT='@',
    COMMA=',',
    LESS = '<',
    GREATER = '>',
    EQUAL='=',
    STAR='*',
    PLUS='+',
    MINUS='-',
    SLASH='/',

    __MULTIBYTE_START=255,

    IDENTIFIER,
    LIT_STRING,
    LIT_NUMBER,

    RIGHT_ARROW,
    LESSEQUAL,
    LESSLESSEQUAL,
    LESSLESS,
    GREATEREQUAL,
    GREATERGREATEREQUAL,
    GREATERGREATER,
    EQUALEQUAL,
    PLUSPLUS,
    MINUSMINUS,
    
    __KEYWORDS_START,
    VAR,
    INT,
    USING,
    NAMESPACE,
    FUNC,
    INCLUDE,
};
class Token{
    private:
        std::string value;
        TokenType type;
    public:
        Token(std::string value, TokenType type){
            this->value = value;
            this->type = type;
        }
        std::string& get_value(){ return this->value; };
        TokenType get_type(){ return this->type; };
        void set_value(std::string value) { this->value = value; }
        void set_type(TokenType type) { this->type = type; }
};
};

#endif // _APEX_TOKEN_HH_
