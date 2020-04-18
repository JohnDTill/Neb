#include "neb_scanner.h"

#include <QDebug>

namespace Neb {

static constexpr ushort USHORT_MAX = 55349;
static constexpr ushort MB_USHORT_CONSTRUCT_SYMBOL = 8284;

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

Scanner::Scanner(const QString& source, QString& err_msg)
    : source(source),
      err_msg(err_msg) {}

Token Scanner::scanToken(){
    #ifndef NEB_IDENTIFIERS_USE_MULTIPLE_CHARS
    if(id_stop) scanImplicitMultiplyIdentifier();
    #endif

    skipWhitespace();
    if(curr == source.size()) return createToken(EndOfFile);

    const ushort c = source[curr++].unicode();

    switch(c){
        NEB_ONE_TO_ONE_CASES_MACRO_EXPANSION
        NEB_NUMBER_CASES_MACRO_EXPANSION
        NEB_SCRIPTED_NUMBER_CASES_MACRO_EXPANSION
        case USHORT_MAX: return scanBigToken();
        case MB_USHORT_CONSTRUCT_SYMBOL: return scanMathBranToken();
        default:
            if(isLetter(c)) return scanIdentifier();
            else if(isSuperscriptLetter(c)) return scanSuperscriptIdentifier();
            else if(isSubscriptLetter(c)) return scanSubscriptIdentifier();

            err_msg = QString("Unrecognized token: ") + source[curr-1];
            return createToken(Error);
    }
}

Token Scanner::createToken(TokenType t){
    return Token(t, curr-1);
}

void Scanner::skipWhitespace(){
    while(curr < source.size()){
        switch (source[curr].unicode()) {
            //This would be the place to ignore comments
            case ' ':
            case '\t':
                curr++;
                break;
            default:
                return;
        }
    }
}

Token Scanner::scanBigToken(){
    if(curr == source.size()){
        err_msg = QString("Unrecognized token: ") + QChar(USHORT_MAX);
        return createToken(Error);
    }

    switch(source[curr++].unicode()){
        NEB_BIG_ONE_TO_ONE_CASES_MACRO_EXPANSION
        default:
            err_msg = "Unrecognized big token: " + source.mid(curr-2, 2);
            return Token(Error, curr-2, 2);
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
            return createToken(Error);
    }
}

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
    auto keyword_lookup = keywords.constFind(source.mid(start, len));

    #ifdef NEB_IDENTIFIERS_USE_MULTIPLE_CHARS
    return Token(keyword_lookup == keywords.end() ? Identifier : keyword_lookup.value(),
                 start,
                 len);
    #else
    if(keyword_lookup == keywords.end()){
        if(len==1) return Token(Identifier, start, len);

        id_stop = curr-1;
        curr = start+1;
        return createToken(Identifier);
    }else{
        return Token(keyword_lookup.value(), start, len);
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

    return Token(SuperscriptIdentifier, start, curr-start);
}

Token Scanner::scanSubscriptIdentifier(){
    QString::size_type start = curr;
    while(curr != source.size() && isSubscriptLetterOrDigit(source[curr].unicode()))
      curr++;

    return Token(SuperscriptIdentifier, start, curr-start);
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
                return Token(Error, start, curr - start);
            }
        }else if(!isDigit(c)){
            break;
        }
        curr++;
    }

    return Token(Number, start, curr - start);
}

Token Scanner::scanZeroNumber(){
    QString::size_type start = curr-1;
    if(curr == source.size()) return Token(Number, start);

    ushort c = source[curr].unicode();
    if(isDigit(c)){
        err_msg = "Numbers cannot have leading zeroes";
        return Token(Error, start, 2);
    }else if(c == '.'){
        if(++curr == source.size() || !source[curr].isDigit()){
            err_msg = "Digits must come after decimal point";
            return Token(Error, start, curr - start);
        }
        while(curr != source.size()){
            ushort c = source[curr].unicode();
            if(isDigit(c)) break;
            curr++;
        }
        return Token(Number, start, curr - start);
    }else{
        return Token(Number, start);
    }
}

Token Scanner::scanSuperscriptNonzeroNumber(){
    QString::size_type start = curr-1;
    while(curr != source.size()){
        if(!isSuperscriptDigit(source[curr].unicode())) break;
        else curr++;
    }

    return Token(SuperscriptNumber, start, curr - start);
}

Token Scanner::scanSuperscriptZeroNumber(){
    QString::size_type start = curr-1;
    if(curr == source.size()) return Token(SuperscriptNumber, start);

    if(isSuperscriptDigit(source[curr].unicode())){
        err_msg = "Numbers cannot have leading zeroes";
        return Token(Error, start, 2);
    }else{
        return Token(SuperscriptNumber, start);
    }
}

Token Scanner::scanSubscriptNonzeroNumber(){
    QString::size_type start = curr-1;
    while(curr != source.size()){
        if(!isSubscriptDigit(source[curr].unicode())) break;
        else curr++;
    }

    return Token(SubscriptNumber, start, curr - start);
}

Token Scanner::scanSubscriptZeroNumber(){
    QString::size_type start = curr-1;
    if(curr == source.size()) return Token(SubscriptNumber, start);

    if(isSubscriptDigit(source[curr].unicode())){
        err_msg = "Numbers cannot have leading zeroes";
        return Token(Error, start, 2);
    }else{
        return Token(SubscriptNumber, start);
    }
}

}
