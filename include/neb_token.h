#ifndef NEB_TOKEN_H
#define NEB_TOKEN_H

#include "neb_tokentype.h"

namespace Neb {

struct Token {
    TokenType type;
    int start;
    int length;

    Token(){}
    Token(TokenType type, int start, int length = 1)
        : type(type), start(start), length(length){}
};

}

#endif // TOKENS_H
