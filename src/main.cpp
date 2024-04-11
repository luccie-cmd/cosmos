#include <fmt/core.h>
#ifdef COMPILE
#include "clopts.hh"
#endif
#include "token.hh"
#include "lexer.hh"
#include "parser.hh"
#include "ast.hh"

using namespace command_line_options;

using options = clopts<
    help<>,
    positional<"File Name", "Path to the file to get compiled", file<>, true>,
    option<"-c", "Enable compilation mode">
>;

int main(int argc, char *argv[]){
    auto opts = options::parse(argc, argv);
    std::string file_content = opts.get<"File Name">()->contents;
    // std::string file_path = opts.get<"File Name">()->path.string();
    apx::Lexer lexer(file_content);
    apx::Parser parser(lexer);
    apx::AstNode *node = parser.next_node();
    node->print();
    return 0;
}