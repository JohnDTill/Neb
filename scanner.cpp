#include "scanner.h"

#include <QDebug>

namespace Neb {

const QMap<QString, TokenType> Scanner::keywords = {
    {"cos", Cos},
    {"sin", Sin}
};

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

    throw(646); //Do this: MEMORY LEAK!
}

void Scanner::scan(){
    while(source_index < source.size()){
        switch(source[source_index++].unicode()){
            case ' ':   break;
            case '\t':  break;
            case '@':   emitToken(At); break;
            case '\\':  emitToken(Backslash); break;
            case '|':   emitToken(Bar); break;
            case 120121:emitToken(Boolean); break; //Do this: Boolean is outside range of UTF-8
            case 8745:  emitToken(Cap); break;
            case 8746:  emitToken(Cup); break;
            case '^':   emitToken(Caret); break;
            case ',':   emitToken(Comma); break;
            case ':':   emitToken(Colon); break;
            case 8743:  emitToken(Conjunction); break;
            case 8224:  emitToken(Dagger); break;
            case 8788:  emitToken(DefEquals); break;
            case 8744:  emitToken(Disjunction); break;
            case 247:   emitToken(Divide); break;
            case '$':   emitToken(Dollar); break;
            case 8901:  emitToken(DotProduct); break;
            case 8214:  emitToken(DoubleBar); break;
            case 8225:  emitToken(DoubleDagger); break;
            case 8252:  emitToken(DoubleExclam); break;
            case 8709:  emitToken(EmptySet); break;
            case '=':   emitToken(Equals); break;
            case '!':   emitToken(Exclam); break;
            case 8707:  emitToken(Exists); break;
            case 8704:  emitToken(ForAll); break;
            case '/':   emitToken(Forwardslash); break;
            case '>':   emitToken(Greater); break;
            case 8805:  emitToken(GreaterEqual); break;
            case 8712:  emitToken(In); break;
            case 8734:  emitToken(Infinity); break;
            case 8484:  emitToken(Integer); break;
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
            case '*':   emitToken(Multiply); break;
            case 8469:  emitToken(Natural); break;
            case '\n':  emitToken(Newline); line++; break;
            case 172:   emitToken(Not); break;
            case 8800:  emitToken(NotEqual); break;
            case 8706:  emitToken(Partial); break;
            case '%':   emitToken(Percent); break;
            case '.':   emitToken(Period); break;
            case 8462:  emitToken(PlanckConst); break;
            case '+':   emitToken(Plus); break;
            case 8473:  emitToken(Prime); break;
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
        case 8594:  emitToken(ArrowAccent); break;
        case 257:   emitToken(BarAccent); break;
        case 8719:  emitToken(BigProduct); break;
        case 8721:  emitToken(BigSum); break;
        case 'b':   emitToken(Binomial); break;
        case 'c':   emitToken(Cases); break;
        case 551:   emitToken(DotAccent); break;
        case 228:   emitToken(DotAccentDouble); break;
        case 8943:  emitToken(DotAccentTriple); break;
        case 916:   emitToken(Dualscript); break;
        case 'f':   emitToken(Fraction); break;
        case 226:   emitToken(Hat); break;
        case 8747:  emitToken(Integral); break;
        case 8862:  emitToken(Matrix); break;
        case 8730:  emitToken(Root); break;
        case '_':   emitToken(Subscript); break;
        case '^':   emitToken(Superscript); break;
        case 227:   emitToken(TildeAccent); break;
        case 'w':   emitToken(UnderscriptedWord); break;

    default:
        fatalError(QString("Unrecognized Escape Code '") + source[source_index-1] + "'");
    }
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

bool Scanner::isIdentifierQChar(const QChar& c) const{
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
