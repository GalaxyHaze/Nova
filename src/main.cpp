#include "../internal/core.h"

int main() {
    const auto stream = nova::file::readSource();
    const auto tokens = Tokenizer::tokenize(stream);
    //const auto Tree = parse(tokens);
    //printTree(Tree);
    printTokens(tokens);

}
