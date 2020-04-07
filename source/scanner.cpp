#include "scanner.h"

#include <QDebug>

namespace Neb {

Scanner::Scanner(const QString& source)
    : source(source) {
    source_index = 0;
}

void Scanner::fatalError(const QString& msg){
    err_msg = "Scanner Error:\n" + msg +
              "\nLine: " + QString::number(line) +
              "\nPosition " + QString::number(source_index);

    if(tokens.empty()){
        emitToken(Identifier);
    }else{
        tokens.front().start = tokens.back().end;
        tokens.front().end = source_index;
    }

    throw(646);
}

void Scanner::scan(){
    source.toStdU32String();
    while(source_index < source.size()){
        switch(source[source_index++].unicode()){
            case ' ':   break;
            case '\t':  break;
            case 55349: scanUTF32(); break;
            case '@':   emitToken(At); break; //DO THIS: Probably want to codegen
            case '\\':  emitToken(Backslash); break;
            case '|':   emitToken(Bar); break;
            case 8745:  emitToken(Cap); break;
            case 8746:  emitToken(Cup); break;
            case '^':   emitToken(Caret); break;
            case ',':   emitToken(Comma); break;
            case ':':   emitToken(Colon); break;
            case 8450:  emitToken(Complex); break;
            case 8743:  emitToken(Conjunction); break;
            case 8224:  emitToken(Dagger); break;
            case 8788:  emitToken(DefEquals); break;
            case 8518:  emitToken(Differential); break;
            case 8744:  emitToken(Disjunction); break;
            case 247:   emitToken(Divide); break;
            case '$':   emitToken(Dollar); break;
            case 8901:  emitToken(DotProduct); break;
            case 8214:  emitToken(DoubleBar); break;
            case 8225:  emitToken(DoubleDagger); break;
            case 8252:  emitToken(DoubleExclam); break;
            case 8709:  emitToken(EmptySet); break;
            case '=':   emitToken(Equals); break;
            case 8364:  emitToken(Euro); break;
            case '!':   emitToken(Exclam); break;
            case 8707:  emitToken(Exists); break;
            case 8704:  emitToken(ForAll); break;
            case '/':   emitToken(Forwardslash); break;
            case '>':   emitToken(Greater); break;
            case 8805:  emitToken(GreaterEqual); break;
            case 8712:  emitToken(In); break;
            case 8734:  emitToken(Infinity); break;
            case 8484:  emitToken(Integer); break;
            case 8747:  emitToken(IntegralUnicode); break;
            case '#':   emitToken(Hash); break;
            case 10216: emitToken(LeftAngle); break;
            case 8592:  emitToken(LeftArrow); break;
            case '{':   emitToken(LeftBrace); break;
            case '[':   emitToken(LeftBracket); break;
            case 8968:  emitToken(LeftCeil); break;
            case 10218: emitToken(LeftDoubleAngle); break;
            case 8970:  emitToken(LeftFloor); break;
            case '(':   emitToken(LeftParen); break;
            case '<':   emitToken(Less); break;
            case 8804:  emitToken(LessEqual); break;
            case 8614:  emitToken(MapsTo); break;
            case 8862:  emitToken(Matrix); break;
            case '-':   emitToken(Minus); break;
            case 8723:  emitToken(MinusPlus); break;
            case '*':   emitToken(Multiply); break;
            case 8711:  emitToken(Nabla); break;
            case 8469:  emitToken(Natural); break;
            case '\n':  emitToken(Newline); line++; break;
            case 172:   emitToken(Not); break;
            case 8800:  emitToken(NotEqual); break;
            case 10752: emitToken(ODot); break;
            case 8855:  emitToken(OuterProduct); break;
            case 8706:  emitToken(Partial); break;
            case '%':   emitToken(Percent); break;
            case 8462:  emitToken(PlanckConst); break;
            case '+':   emitToken(Plus); break;
            case 177:   emitToken(PlusMinus); break;
            case 163:   emitToken(PoundSterling); break;
            case 8473:  emitToken(Prime); break;
            case 8733:  emitToken(Proportional); break;
            case 8461:  emitToken(Quaternion); break;
            case 8474:  emitToken(Rational); break;
            case 8477:  emitToken(Real); break;
            case 10217: emitToken(RightAngle); break;
            case 8594:  emitToken(RightArrow); break;
            case '}':   emitToken(RightBrace); break;
            case ']':   emitToken(RightBracket); break;
            case 8969:  emitToken(RightCeil); break;
            case 10219: emitToken(RightDoubleAngle); break;
            case 8971:  emitToken(RightFloor); break;
            case ')':   emitToken(RightParen); break;
            case 8730:  emitToken(Root); break;
            case ';':   emitToken(Semicolon); break;
            case 9205:  emitToken(SpecialClose); break;
            case 9204:  emitToken(SpecialOpen); break;
            case '\'':  emitToken(Tick); break;
            case '~':   emitToken(Tilde); break;
            case 215:   emitToken(Times); break;
            case 8868:  emitToken(Transpose); break;
            case 8284:  scanEscapeCode(); break;

            case '.':
                if(match('.')){
                    consume('.');
                    emitToken(SuspensionPoint);
                }else{
                    emitToken(Period);
                }
                break;

            default:
                QChar c = source[source_index-1];
                if(c.isDigit()) scanNumber();
                else if(isIdentifierQChar(c)) scanText();
                else fatalError(QString("Unrecognized character '") + c + "'");
        }
    }
}

void Scanner::scanEscapeCode(){
    emitToken(SpecialEscape);
    if(source_index >= source.size()) fatalError("Reached end of file while looking for Escape Code.");

    switch(source[source_index++].unicode()){
        case 8594:  emitToken(AccentArrow); break;
        case 257:   emitToken(AccentBar); break;
        case 259:   emitToken(AccentBreve); break;
        case 551:   emitToken(AccentDot); break;
        case 228:   emitToken(AccentDdot); break;
        case 8943:  emitToken(AccentDddot); break;
        case 226:   emitToken(AccentHat); break;
        case 227:   emitToken(AccentTilde); break;
        case 8719:  emitToken(BigProduct); break;
        case 8721:  emitToken(BigSum); break;
        case 'b':   emitToken(Binomial); break;
        case 'c':   emitToken(Cases); break;
        case 916:   emitToken(Dualscript); break;
        case 'f':   emitToken(Fraction); break;
        case 8747:  emitToken(Integral); break;
        case 8862:  emitToken(Matrix); break;
        case 8730:  emitToken(Root); break;
        case '_':   emitToken(Subscript); break;
        case '^':   emitToken(Superscript); break;
        case 'w':   emitToken(UnderscriptedWord); break;
        case '|':   consume('|'); emitToken(Bar); break;
        case 8214:  consume(8214); emitToken(DoubleBar); break;
        case 8968:  consume(8969); emitToken(LeftCeil); break;
        case 8970:  consume(8971); emitToken(LeftFloor); break;
        case '(':   consume(')'); emitToken(LeftParen); break;
        case '[':   consume(']'); emitToken(LeftBracket); break;

    default:
        fatalError(QString("Unrecognized Escape Code '") + source[source_index-1] + "'");
    }
}

void Scanner::scanUTF32(){
    if(source_index >= source.size()) fatalError("End of file while reading UTF-32 character");
    QString UTF32_char = source.mid(source_index++ - 1,2);

    auto lookup = UTF32_map.constFind(UTF32_char);
    if(lookup == UTF32_map.end()) fatalError("Invalid UTF32 char: " + UTF32_char);

    emitToken(lookup.value());
}

void Scanner::scanNumber(){
    QString::size_type start = source_index - 1;

    if(source_index < source.size())
        while(source[source_index].isDigit() && advanceScanner());
    if(source_index < source.size() && source[source_index] == '.')
        while(advanceScanner() && source[source_index].isDigit());

    emitToken(Number, start);
}

void Scanner::scanText(){
    QString::size_type start = source_index - 1;

    if(source_index < source.size())
        while(isIdentifierQChar(source[source_index]) && advanceScanner());
    QString str = source.mid(start, source_index-start);

    auto keyword_lookup = keywords.find(str);
    if(keyword_lookup != keywords.end()) emitToken(keyword_lookup.value(), start);
    else if(identifiers_use_multiple_chars) emitToken(Identifier, start);
    else for(int i = start; i < source_index; i++) emitToken(Identifier, i, i+1);
}

bool Scanner::match(const QChar& c){
    bool is_match = source_index < source.size() && source[source_index] == c;
    if(is_match) source_index++;
    return is_match;
}

void Scanner::consume(const QChar& c){
    if(!match(c)) fatalError(QString("Expected char '") + c + '\'');
}

bool Scanner::isIdentifierQChar(const QChar& c) const{
    if(c > 1000) return false; //Some doublestruck letters such as 'ⅆ' are counted as letters.

    if(identifiers_use_multiple_chars)
        return c.isLetter() || c.isNumber() || c == '_' || (c >= 913 && c <= 937) || (c >= 945 && c <= 969);
    else
        return c.isLetter() || (c >= 913 && c <= 937) || (c >= 945 && c <= 969);
}

bool Scanner::advanceScanner(){
    return ++source_index < source.size();
}

void Scanner::emitToken(const TokenType& t){
    emitToken(t, source_index-1);
}

void Scanner::emitToken(const TokenType& t, QString::size_type start){
    Token token;
    token.type = t;
    token.start = start;
    token.end = source_index;

    tokens.push_back(token);
}

void Scanner::emitToken(const TokenType& t, QString::size_type start, QString::size_type end){
    Token token;
    token.type = t;
    token.start = start;
    token.end = end;

    tokens.push_back(token);
}

}
