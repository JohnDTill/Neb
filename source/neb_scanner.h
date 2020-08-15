#ifndef NEB_SCANNER_H
#define NEB_SCANNER_H

#define NEB_IDENTIFIERS_USE_MULTIPLE_CHARS

#include "neb_token.h"
class QString;

namespace Neb {

class Scanner{
private:
    const QString& source;
    int curr = 0;
    #ifndef NEB_IDENTIFIERS_USE_MULTIPLE_CHARS
    int id_stop;
    #endif
    QString& err_msg;

public:
    Scanner(const QString& source, QString& err_msg);
    Token scanToken();

private:
    Token createToken(TokenType t);
    void skipWhitespace();
    void skipLineComment();
    Token scanBigToken();
    Token scanMathBranToken();
    Token scanIdentifier();
    #ifndef NEB_IDENTIFIERS_USE_MULTIPLE_CHARS
    Token scanImplicitMultiplyIdentifier();
    #endif
    Token scanSuperscriptIdentifier();
    Token scanSubscriptIdentifier();
    Token scanNonzeroNumber();
    Token scanZeroNumber();
    Token scanSuperscriptNonzeroNumber();
    Token scanSuperscriptZeroNumber();
    Token scanSubscriptNonzeroNumber();
    Token scanSubscriptZeroNumber();
};

}

#endif // SCANNER_H
