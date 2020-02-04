#ifndef TOKENS_H
#define TOKENS_H

#include "tokentype.h"
#include <QString>

namespace Neb {

struct Token {
    TokenType type;
    QString::size_type start;
    QString::size_type end;
};

}

#endif // TOKENS_H
