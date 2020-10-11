#include "neb_scanner.h"

#include <QString>
#include <QMathBran.h>

namespace Neb {

static constexpr ushort USHORT_MAX = 55349;

static bool isLetter(ushort c){
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= 945 && c <= 969) || //α-ω
           (c >= 913 && c <= 937)    //Α-Ω
           #ifdef NEB_IDENTIFIERS_USE_MULTIPLE_CHARS
           || c == '_'
           #endif
           ;
}

static bool isDigit(ushort c){
    return c >= '0' && c <= '9';
}

static bool isSuperscriptLetter(ushort c){
    return c == 8305 || //ⁱ
           c == 8319 || //ⁿ
           c == 7520 || //ᵠ
           c == 7615;   //ᶿ
}

static bool isSuperscriptDigit(ushort c){
    return  c == 8304             || //⁰
            c == 185              || //¹
           (c == 178 || c == 179) || //²⁻³
           (c >= 8308 && c <= 8313); //⁴⁻⁹
}

static bool isSubscriptLetter(ushort c){
    return (c >= 8336 && c <= 8339) || //ₐ,ₑ,ₒ,ₓ
           (c >= 8341 && c <= 8348);   //ₕ,ₖ,ₗ,ₘ,ₙ,ₚ,ₛ,ₜ
}

static bool isSubscriptDigit(ushort c){
    return c >= 8320 && c <= 8329; //₀₋₉
}

#ifdef NEB_IDENTIFIERS_USE_MULTIPLE_CHARS
static bool isLetterOrDigit(ushort c){
    return isLetter(c) || isDigit(c);
}

static bool isSuperscriptLetterOrDigit(ushort c){
    return isSuperscriptLetter(c) || isSuperscriptDigit(c);
}

static bool isSubscriptLetterOrDigit(ushort c){
    return isSubscriptLetter(c) || isSuperscriptDigit(c);
}
#endif

Scanner::Scanner(const QString& source, QString& err_msg, int& err_start, int& err_end)
    : source(source),
      err_msg(err_msg),
      err_start(err_start),
      err_end(err_end) {
    Q_ASSERT(MathBran::isWellFormed(source));
}

Token Scanner::scanToken(){
    #ifndef NEB_IDENTIFIERS_USE_MULTIPLE_CHARS
    if(id_stop) scanImplicitMultiplyIdentifier();
    #endif

    skipWhitespace();
    if(curr >= source.size()) return createToken(EndOfFile);

    const ushort c = source[curr++].unicode();

    switch(c){
        NEB_ONE_TO_ONE_CASES_MACRO_EXPANSION
        NEB_NUMBER_CASES_MACRO_EXPANSION
        NEB_SCRIPTED_NUMBER_CASES_MACRO_EXPANSION
        case USHORT_MAX: return scanBigToken();
        case MB_USHORT_CONSTRUCT_SYMBOL: return scanMathBranToken();
        case '"': return scanString();
        case '\n': line++; return Token(Newline, line-1, curr-1);
        default:
            if(isLetter(c)) return scanIdentifier();
            else if(isSuperscriptLetter(c)) return scanSuperscriptIdentifier();
            else if(isSubscriptLetter(c)) return scanSubscriptIdentifier();

            err_msg = QString("Unrecognized token: ") + source[curr-1];
            err_start = curr-1;
            err_end = curr;
            return createToken(Error);
    }
}

Token Scanner::createToken(TokenType t){
    return Token(t, line, curr-1);
}

void Scanner::skipWhitespace(){
    while(curr < source.size()){
        switch (source[curr].unicode()) {
            case ' ':
            case '\t':
                curr++;
                break;
            case '/':
                if(curr+1 < source.size() && source[curr+1] == '/') skipLineComment();
                return;
            default:
                return;
        }
    }
}

void Scanner::skipLineComment(){
    while(curr < source.size() && source[curr] != '\n') curr++;
}

Token Scanner::scanBigToken(){
    if(curr == source.size()){
        err_msg = QString("Unrecognized token: ") + QChar(USHORT_MAX);
        err_start = curr-1;
        err_end = curr;
        return createToken(Error);
    }

    switch(source[curr++].unicode()){
        NEB_BIG_ONE_TO_ONE_CASES_MACRO_EXPANSION
        default:
            err_msg = "Unrecognized big token: " + source.mid(curr-2, 2);
            err_start = curr-2;
            err_end = curr;
            return Token(Error, line, curr-2, 2);
    }
}

Token Scanner::scanMathBranToken(){
    if(curr == source.size()){
        err_msg = QString("Construct token '⁜' at end of file");
        return createToken(Error);
    }

    switch(source[curr++].unicode()){
        NEB_MATHBRAN_CASES_MACRO_EXPANSION
        default:
            err_msg = QString("Unrecognized token: ") + source[curr-1];
            err_start = curr-1;
            err_end = curr;
            return createToken(Error);
    }
}

NEB_DECLARE_KEYWORD_SEARCH

Token Scanner::scanIdentifier(){
    QString::size_type start = curr-1;

    #ifdef NEB_IDENTIFIERS_USE_MULTIPLE_CHARS
    while(curr != source.size() && isLetterOrDigit(source[curr].unicode()))
        curr++;
    #else
    while(curr != source.size() && isLetter(source[curr].unicode()))
        curr++;
    #endif

    QString::size_type len = curr - start;
    TokenType t = getTextLexemeType(source.midRef(start, len));

    #ifdef NEB_IDENTIFIERS_USE_MULTIPLE_CHARS
    return Token(t, line, start, len);
    #else
    if(t == Identifier){
        if(len==1) return Token(Identifier, line, start, len);

        id_stop = curr-1;
        curr = start+1;
        return createToken(Identifier);
    }else{
        return Token(t, line, start, len);
    }
    #endif
}

#ifndef NEB_IDENTIFIERS_USE_MULTIPLE_CHARS
Token Scanner::scanImplicitMultiplyIdentifier(){
    if(curr == id_stop) id_stop = 0;
    return createToken(Identifier);
}

//Do not allow keywords in unicode script
Token Scanner::scanSuperscriptIdentifier(){
    return createToken(SuperscriptIdentifier);
}

Token Scanner::scanSubscriptIdentifier(){
    return createToken(SubscriptIdentifier);
}
#else
Token Scanner::scanSuperscriptIdentifier(){
    QString::size_type start = curr;
    while(curr != source.size() && isSuperscriptLetterOrDigit(source[curr].unicode()))
        curr++;

    return Token(SuperscriptIdentifier, line, start, curr-start);
}

Token Scanner::scanSubscriptIdentifier(){
    QString::size_type start = curr;
    while(curr != source.size() && isSubscriptLetterOrDigit(source[curr].unicode()))
      curr++;

    return Token(SuperscriptIdentifier, line, start, curr-start);
}
#endif

Token Scanner::scanNonzeroNumber(){
    QString::size_type start = curr-1;
    bool decimal_available = true;
    while(curr != source.size()){
        ushort c = source[curr].unicode();
        if(decimal_available && c == '.'){
            decimal_available = false;
            if(++curr == source.size() || !isDigit(source[curr].unicode())){
                err_msg = "Digits must come after decimal point";
                err_start = start;
                err_end = curr;
                return Token(Error, line, start, curr - start);
            }
        }else if(!isDigit(c)){
            break;
        }
        curr++;
    }

    return Token(Number, line, start, curr - start);
}

Token Scanner::scanZeroNumber(){
    QString::size_type start = curr-1;
    if(curr == source.size()) return Token(Number, line, start);

    ushort c = source[curr].unicode();
    if(isDigit(c)){
        err_msg = "Numbers cannot have leading zeroes";
        err_start = start;
        err_end = curr;
        return Token(Error, line, start, 2);
    }else if(c == '.'){
        if(++curr == source.size() || !source[curr].isDigit()){
            err_msg = "Digits must come after decimal point";
            err_start = start;
            err_end = curr;
            return Token(Error, line, start, curr - start);
        }
        while(curr != source.size()){
            ushort c = source[curr].unicode();
            if(!isDigit(c)) break;
            curr++;
        }
        return Token(Number, line, start, curr - start);
    }else{
        return Token(Number, line, start);
    }
}

Token Scanner::scanSuperscriptNonzeroNumber(){
    QString::size_type start = curr-1;
    while(curr != source.size()){
        if(!isSuperscriptDigit(source[curr].unicode())) break;
        else curr++;
    }

    return Token(SuperscriptNumber, line, start, curr - start);
}

Token Scanner::scanSuperscriptZeroNumber(){
    QString::size_type start = curr-1;
    if(curr == source.size()) return Token(SuperscriptNumber, line, start);

    if(isSuperscriptDigit(source[curr].unicode())){
        err_msg = "Numbers cannot have leading zeroes";
        err_start = start;
        err_end = curr;
        return Token(Error, line, start, 2);
    }else{
        return Token(SuperscriptNumber, line, start);
    }
}

Token Scanner::scanSubscriptNonzeroNumber(){
    QString::size_type start = curr-1;
    while(curr != source.size()){
        if(!isSubscriptDigit(source[curr].unicode())) break;
        else curr++;
    }

    return Token(SubscriptNumber, line, start, curr - start);
}

Token Scanner::scanSubscriptZeroNumber(){
    QString::size_type start = curr-1;
    if(curr == source.size()) return Token(SubscriptNumber, line, start);

    if(isSubscriptDigit(source[curr].unicode())){
        err_msg = "Numbers cannot have leading zeroes";
        err_start = start;
        err_end = curr;
        return Token(Error, line, start, 2);
    }else{
        return Token(SubscriptNumber, line, start);
    }
}

Token Scanner::scanString(){
    QString::size_type start = curr;

    while(curr < source.size() && source[curr] != '"'){
        if(source[curr] == '\n' && source[curr-1] != '\\'){
            err_msg = "String not terminated";
            err_start = start-1;
            err_end = curr;
            return Token(Error, line, start);
        }
        curr++;
    }
    curr++;

    if(curr >= source.size()){
        err_msg = "Reached end of file while scanning string";
        err_start = start-1;
        err_end = source.size()-1;
        return Token(Error, line, start);
    }else{
        return Token(String, line, start, curr-start-1);
    }
}

}
