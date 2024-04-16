#include "parser.hh"
#include <memory>

namespace cosmos {
    AstNode *Parser::next_node() {
        AstNode* node;
        switch(current.get_type()) {
            case TokenType::SEMICOLON: {
                // Do nothing since semicolons aren't important anyway
                this->consume();
                // Call the function again
                return this->next_node();
            } break;
            case TokenType::AT: {
                // Let the preprocessor handle it
                node = this->preprocessor();
            } break;
            case TokenType::USING: {
                node = this->parse_using();
            } break;
            case TokenType::VAR: {
                node = this->parse_var();
            } break;
            case TokenType::LIT_NUMBER: {
                node = this->parse_binary_expr();
            } break;
            default: {
                fmt::print("Encountered invalid token with value {} type = {}\n", current.get_value(), (int)current.get_type());
                exit(1);
            }
        }
        return node;
    }
    AstNode* Parser::parse_string(){
        std::string value = this->expect(TokenType::LIT_STRING, fmt::format("Expected a string literal but got {}\n", this->current.get_value())).get_value();
        NodeStringLit *strlit = new NodeStringLit(value);
        AstNode *return_node = new AstNode(AstType::STRING_LITERAL);
        return_node->setNode(strlit);
        return return_node;
    }
    AstNode *Parser::preprocessor() {
        this->expect(TokenType::AT, fmt::format("Expected `@` but got {}\n", this->current.get_value()));
        AstNode* node;
        switch (this->current.get_type()) {
            case TokenType::INCLUDE: {
                this->consume();
                AstNode *path = this->parse_string();
                if (path->get_type() != AstType::STRING_LITERAL) {
                    fmt::print("ERROR: Expected string literal but got {}\n", static_cast<int>(path->get_type()));
                    exit(1);
                }
                NodeInclude *include_node = new NodeInclude(path);
                AstNode *return_node = new AstNode(AstType::INCLUDE);
                return_node->setNode(include_node);
                node = return_node;
            } break;
            default: {
                fmt::print("Invalid preprocessor expression with value: {}\n", this->current.get_value());
                exit(1);
            }
        }
        return node;
    }
    std::vector<std::pair<TokenType, ConstructorType>> ConstructorTypeLookupTable = {
        {TokenType::NAMESPACE, ConstructorType::NAMESPACE}
    };
    ConstructorType Parser::parse_constructor_type(){
        for(auto e : ConstructorTypeLookupTable){
            if(e.first == this->current.get_type()){
                this->consume();
                return e.second;
            }
        }
        return ConstructorType::INVALID;
    }
    AstNode* Parser::parse_using(){
        this->expect(TokenType::USING, fmt::format("Expected using but got {}\n", this->current.get_value()));
        ConstructorType constructor_type = this->parse_constructor_type();
        if(constructor_type == ConstructorType::INVALID){
            fmt::print("Expected constructor type but got {}\n", this->current.get_value());
            fmt::print("Valid constructor types are: \n");
            fmt::print("    namespace\n");
            exit(1);
        }

        std::string name = this->expect(TokenType::IDENTIFIER, fmt::format("Expected a namespace name (identifier) but got {}\n", this->current.get_value())).get_value();
        this->expect(TokenType::SEMICOLON, fmt::format("Expected a semicolon (';') but got {}\n", this->current.get_value()));

        AstNode *node = new AstNode(AstType::USING);
        NodeUsing *using_node = new NodeUsing(name, constructor_type);
        node->setNode(using_node);
        return node;
    }
    std::vector<std::pair<TokenType, Types>> TypesLookupTable = {
        {TokenType::INT, Types::INT},
        {TokenType::STR, Types::STRING},
    };
    Types Parser::parse_type(){
        for(auto e : TypesLookupTable){
            if(e.first == this->current.get_type()){
                this->consume();
                return e.second;
            }
        }
        return Types::INVALID;
    }
    std::vector<std::pair<TokenType, AssignOp>> AssignLookupTable = {
        {TokenType::GREATERGREATEREQUAL, AssignOp::SHREQUAL},
        {TokenType::LESSLESSEQUAL, AssignOp::SHLEQUAL},
        {TokenType::EQUAL, AssignOp::EQUAL},
    };
    NodeAssignOp *Parser::parse_assign_op(){
        for(auto e : AssignLookupTable){
            if(e.first == this->current.get_type()){
                this->consume();
                return new NodeAssignOp(e.second);
            }
        }
        return new NodeAssignOp(AssignOp::INVALID);
    }
    AstNode* Parser::parse_var(){
        this->expect(TokenType::VAR, fmt::format("Expected var but got {}\n", this->current.get_value()));
        std::string name = this->expect(TokenType::IDENTIFIER, fmt::format("Expected a variable name (identifier) but got {}\n", this->current.get_value())).get_value();
        this->expect(TokenType::COLON, fmt::format("Expected a colon (':') but got {}\n", this->current.get_value()));
        Types type = this->parse_type();
        // It's only a decleration
        AstNode *node = new AstNode(AstType::VAR_DECL);
        if(this->current.get_type() == TokenType::SEMICOLON){
            this->consume();
            NodeVarDecl *vardecl_node = new NodeVarDecl(name, type, false, nullptr);
            node->setNode(vardecl_node);
        } else{
            NodeAssignOp *assignop = this->parse_assign_op();
            AstNode *expr = this->parse_expr();
            this->expect(TokenType::SEMICOLON, fmt::format("Expected ';' but got {}\n", this->current.get_value()));
            NodeVarDecl* var_decl = new NodeVarDecl(name, type, true, nullptr);
            NodeVarAssign* var_assign = new NodeVarAssign(name, assignop, expr);
            var_decl->set_Nassign(var_assign);
            node->setNode(var_decl);
        }
        return node;
    }
    std::vector<std::pair<TokenType, Operator>> OperatorLookupTable = {
        {TokenType::PLUS, Operator::PLUS},
        {TokenType::MINUS, Operator::MINUS},
        {TokenType::STAR, Operator::MULTIPLY},
        {TokenType::SLASH, Operator::DIVIDE},
    };
    Operator Parser::parse_operator(){
        for(auto e : OperatorLookupTable){
            if(e.first == this->current.get_type()){
                this->consume();
                return e.second;
            }
        }
        return Operator::INVALID;
    }
    AstNode* Parser::parse_add_binary_expr(){
        AstNode* lhs = this->parse_mul_binary_expr();
        while (this->current.get_type() == TokenType::PLUS || this->current.get_type() == TokenType::MINUS) {
            Operator op = this->parse_operator();
            AstNode* rhs = this->parse_mul_binary_expr();
            NodeExpr *expr = new NodeExpr(ExpressionType::BINARY);
            expr->set_binary_expr(new NodeBinaryExpr(lhs, op, rhs));
            // FIXME: This is stupid don't do this make this more compressed
            AstNode* tmp = new AstNode(AstType::EXPRESSION);
            tmp->setNode(expr);
            lhs = tmp;
        }
        return lhs;
    }
    AstNode* Parser::parse_mul_binary_expr(){
        AstNode* lhs = this->parse_primary_expr();
        while (this->current.get_type() == TokenType::STAR || this->current.get_type() == TokenType::SLASH) {
            Operator op = this->parse_operator();
            AstNode* rhs = this->parse_primary_expr();
            NodeExpr *expr = new NodeExpr(ExpressionType::BINARY);
            expr->set_binary_expr(new NodeBinaryExpr(lhs, op, rhs));
            // FIXME: This is stupid don't do this make this more compressed
            AstNode* tmp = new AstNode(AstType::EXPRESSION);
            tmp->setNode(expr);
            lhs = tmp;
        }
        return lhs;
    }
    AstNode* Parser::parse_binary_expr(){
        return this->parse_add_binary_expr();
    }
    AstNode* Parser::parse_primary_expr(){
        if(this->current.get_type() == TokenType::LIT_NUMBER){
            uint64_t value = std::stoull(current.get_value());
            this->consume();
            NodeIntLit *intlit = new NodeIntLit(value);
            AstNode *return_node = new AstNode(AstType::INTEGER_LITERAL);
            return_node->setNode(intlit);
            return return_node;
        } else if(this->current.get_type() == TokenType::LIT_STRING){
            return this->parse_string();
        } else if(this->current.get_type() == TokenType::OPENPAREN){
            this->consume();
            AstNode* ret = this->parse_expr();
            this->expect(TokenType::CLOSEPAREN, "Unexpected token found inside parenthesised expression. Expected closing parenthesis (')')");
            return ret;
        }
        fmt::print("You fucking stupid bitch {}\n", this->current.get_value());
        exit(1);
    }
    AstNode* Parser::parse_expr(){
        // TODO: Handle call expressions
        AstNode* node = this->parse_binary_expr();
        return node;
    }
}
