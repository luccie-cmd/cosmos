#include "parser.hh"
#include <memory>

namespace apx {
    AstNode *Parser::next_node() {
        AstNode* node;
        switch(current.get_type()) {
            case TokenType::AT: {
                // Let the preprocessor handle it
                node = this->preprocessor();
            } break;
            case TokenType::LIT_STRING: {
                std::string value = current.get_value();
                this->consume();
                NodeStringLit *strlit = new NodeStringLit(value);
                AstNode *return_node = new AstNode(AstType::STRING_LITERAL);
                return_node->setNode(strlit);
                node = return_node;
            } break;
            default: {
                fmt::print("Encountered invalid token with value {}\n", current.get_value());
                exit(1);
            }
        }
        return node;
    }

    AstNode *Parser::preprocessor() {
        this->expect(TokenType::AT, fmt::format("Expected `@` but got {}\n", this->current.get_value()));
        AstNode* node;
        switch (this->current.get_type()) {
            case TokenType::INCLUDE: {
                this->consume();
                AstNode *path = this->next_node();
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
}
