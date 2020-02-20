#ifndef SCANNER_H
#define SCANNER_H

#include "tokens.h"
#include <vector>

namespace Neb {

class Parser;

class Scanner{
private:
    friend Parser;
    static constexpr bool identifiers_use_multiple_chars = false;
    const QString& source;
    QString::size_type source_index = 0;
    uint64_t line = 0;
    std::vector<Token> tokens;
    QString err_msg;

private:
    Scanner(const QString& source);

private:
    [[noreturn]] void fatalError(const QString& msg);
    void scan();
    void scanEscapeCode();
    void scanUTF32();
    void scanNumber();
    void scanText();
    bool isIdentifierQChar(const QChar& c) const;
    bool advanceScanner();
    void emitToken(const TokenType& t);
    void emitToken(const TokenType& t, QString::size_type start);
    void emitToken(const TokenType& t, QString::size_type start, QString::size_type end);
};

}

#endif // SCANNER_H
