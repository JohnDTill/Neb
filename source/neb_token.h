#ifndef NEB_TOKEN_H
#define NEB_TOKEN_H

#include "neb_tokentype.h"
#include <QString>

namespace Neb {

struct Token {
    TokenType type;
    QString::size_type start;
    QString::size_type length;

    Token(){}
    Token(TokenType type, QString::size_type start, QString::size_type length = 1)
        : type(type), start(start), length(length){}
};

}

#endif // TOKENS_H
