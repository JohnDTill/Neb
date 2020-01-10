#include "parser.h"

#include <QDebug>
#define DO_THIS(message) \
    qDebug() << "DO THIS: " << message << ", file " << __FILE__ << " line " << __LINE__;

std::vector<Parser::Node*> Parser::parse(const QString& source){
    Parser parser(source);
    parser.scan();
    parser.parse();

    return parser.statements;
}

QString Parser::toDOT(const Node& n){
    QString str;
    QTextStream out(&str);

    out << "digraph{\n";
    uint64_t curr = 0;
    writeDOT(out, n, curr);
    out << "}";

    return str;
}

QString Parser::toDOT(const std::vector<Node*>& nodes){
    QString str;
    QTextStream out(&str);

    out << "digraph{\n";
    uint64_t curr = 0;
    for(Node* n : nodes) writeDOT(out, *n, curr);
    out << "}";

    return str;
}

Parser::Parser(const QString& source)
    : source(source) {
    source_index = 0;
}

uint64_t Parser::writeDOT(QTextStream& out, const Node& n, uint64_t& curr){
    uint64_t id = curr++;

    out << "\tn" << QString::number(id) << "[label=\"" << n.label;
    if(!n.subtext.isEmpty()) out << ": " << n.subtext;
    out << "\"]\n";

    for(Node* n : n.children){
        uint64_t child_id = writeDOT(out, *n, curr);
        out << "\tn" << QString::number(id) << "->n" << QString::number(child_id) << '\n';
    }

    return id;
}

const QMap<QString, Parser::TokenType> Parser::keywords = {
    {"cos", Cos},
    {"sin", Sin}
};

void Parser::fatalError(const QString& msg){
    qDebug() << "FATAL ERROR: " << msg << "\nPosition " << source_index;
    exit(0);
}

Parser::Node* Parser::createNode(const QChar& label){
    Node* n = new Node;
    n->label = label;

    return n;
}

Parser::Node* Parser::createNode(const QChar& label, Node* child){
    Node* n = new Node;
    n->label = label;
    n->children.push_back(child);

    return n;
}

Parser::Node* Parser::createNode(const QChar& label, Node* lhs, Node* rhs){
    Node* n = new Node;
    n->label = label;
    n->children.push_back(lhs);
    n->children.push_back(rhs);

    return n;
}

Parser::Node* Parser::createNode(const QChar& label, std::vector<Node*> children){
    Node* n = new Node;
    n->label = label;
    n->children = children;

    return n;
}

void Parser::parse(){
    while(match(Newline));
    while(token_index < tokens.size()){
        statements.push_back( statement() );
        if(token_index < tokens.size()) consume({Newline, Comma});
        while(match(Newline));
    }
}

void Parser::consume(const TokenType& t){
    if(token_index >= tokens.size()) fatalError("Reached end of token stream while scanning");
    if(t != tokens[token_index++].type) fatalError(QString("Expected token type '") + t + "'");
}

void Parser::consume(const std::vector<Parser::TokenType>& types){
    if(token_index >= tokens.size()) fatalError("Reached end of token stream while scanning");
    TokenType t = tokens[token_index++].type;
    for(TokenType type : types) if(type == t) return;
    fatalError(QString("Invalid token type '") + t + "'");
}

bool Parser::match(const Parser::TokenType& t){
    if(token_index >= tokens.size()) return false;
    if(t == tokens[token_index].type){
        token_index++;
        return true;
    }else{
        return false;
    }
}

bool Parser::match(const std::vector<Parser::TokenType>& types){
    if(token_index >= tokens.size()) return false;
    TokenType t = tokens[token_index].type;
    for(TokenType type : types) if(type == t){
        token_index++;
        return true;
    }

    return false;
}

bool Parser::peek(const Parser::TokenType& t) const{
    return token_index < tokens.size() && t == tokens[token_index].type;
}

bool Parser::peek(const std::vector<Parser::TokenType>& types) const{
    if(token_index >= tokens.size()) return false;
    TokenType t = tokens[token_index].type;
    for(TokenType type : types) if(type == t) return true;
    return false;
}

void Parser::skipPastSpecialClose(){
    uint8_t level = 0;
    for(;;){
        if(match(SpecialClose)){
            if(level == 0) return;
            else level--;
        }else if(match(SpecialOpen)){
            level++;
        }else if(token_index < tokens.size()){
            token_index++;
        }else{
            fatalError("Reached end of file scanning for SpecialClose.");
        }
    }
}

Parser::Node* Parser::statement(){
    Node* n = expression();

    if(match(Equals)){
        match(Newline);
        n = createNode('=', n, expression());
        while(match(Equals)){
            match(Newline);
            n->children.push_back(expression());
        }

        return n;
    }else if(match({Less, LessEqual})){
        QChar c = source[tokens[token_index-1].start];
        match(Newline);
        n = createNode(c, n, expression());
        while(match({Less, LessEqual})){
            QChar c = source[tokens[token_index-1].start];
            match(Newline);
            n = createNode(c, n, expression());
        }

        return n;
    }else if(match({Greater, GreaterEqual})){
        QChar c = source[tokens[token_index-1].start];
        match(Newline);
        n = createNode(c, n, expression());
        while(match({Greater, GreaterEqual})){
            QChar c = source[tokens[token_index-1].start];
            match(Newline);
            n = createNode(c, n, expression());
        }

        return n;
    }else if(match(In)){
        QChar c = source[tokens[token_index-1].start];
        return createNode(c, n, expression());
    }else if(match({DefEquals, NotEqual})){
        QChar c = source[tokens[token_index-1].start];
        return createNode(c, n, expression());
    }else{
        return createNode('E', n);
    }
}

Parser::Node* Parser::expression(){
    return conjunction();
}

Parser::Node* Parser::conjunction(){
    Node* expr = disjunction();
    if(match(Conjunction)){
        match(Newline);
        expr = createNode(QChar(8743), expr, disjunction());
        while(match(Conjunction)){
            match(Newline);
            expr->children.push_back(disjunction());
        }
    }

    return expr;
}

Parser::Node* Parser::disjunction(){
    Node* expr = addition();
    if(match(Disjunction)){
        match(Newline);
        expr = createNode(QChar(8744), expr, addition());
        while(match(Disjunction)){
            match(Newline);
            expr->children.push_back(addition());
        }
    }

    return expr;
}

Parser::Node* Parser::addition(){ //Left associative (subtraction is anti-commutative)
    Node* expr = multiplication();
    while(match({Plus, Minus, Cap, Cup})){
        QChar c = source[tokens[token_index-1].start];
        match(Newline);
        expr = createNode(c, expr, multiplication());
    }

    return expr;
}

Parser::Node* Parser::multiplication(){
    Node* expr = leftUnary();
    while(match({Backslash, Divide, DotProduct, Forwardslash, Multiply, Percent, Times})){
        QChar c = source[tokens[token_index-1].start];
        match(Newline);
        expr = createNode(c, expr, leftUnary());
    }

    return expr;
}

Parser::Node* Parser::leftUnary(){
    if(match( {Minus, Not} )){
        QChar c = source[tokens[token_index-1].start];
        return createNode(c, rightUnary());
    }else{
        return rightUnary();
    }
}

Parser::Node* Parser::rightUnary(){
    Node* expr = exponent();
    while(match( {Exclam, DoubleExclam, Tick} )){
        QChar c = source[tokens[token_index-1].start];
        expr = createNode(c, expr);
    }

    return expr;
}

Parser::Node* Parser::exponent(){
    Node* expr = grouping();
    if(match(Caret))
        expr = createNode('^', expr, leftUnary());
    return expr;
}

Parser::Node* Parser::grouping(){
    if(match(LeftParen)){
        Node* expr = expression();
        if(match(Comma)){
            //Exclusive start of range
            expr = createNode('R', expr, expression());

            if(match(RightParen)){
                expr->subtext = "()";
            }else{
                consume(RightBracket);
                expr->subtext = "(]";
            }

            return expr;
        }else{
            consume(RightParen);
            return createNode('(', expr);
        }
    }else if(match(LeftBracket)){
        Node* expr = expression();
        if(match(Comma)){
            //Inclusive start of range
            expr = createNode('R', expr, expression());

            if(match(RightParen)){
                expr->subtext = "[)";
            }else{
                consume(RightBracket);
                expr->subtext = "[]";
            }

            return expr;
        }else{
            consume(RightBracket);
            return createNode('[', expr);
        }
    }else if(match(LeftBrace)){
        Node* expr = createNode('[', expression());
        while(match(Comma)){
            match(Newline);
            expr->children.push_back( expression() );
        }
        consume(RightBrace);

        return expr;
    }else if(match(Bar)){
        Node* expr = expression();
        consume(Bar);
        return createNode('|', expr);
    }else if(match(DoubleBar)){
        Node* expr = expression();
        consume(DoubleBar);
        return createNode(QChar(8214), expr);
    }else if(match(SpecialEscape)){
        return escape();
    }else{
        return terminal();
    }
}

Parser::Node* Parser::callArgs(){
    Node* n = createNode('A');

    if(!match(RightParen)){
        match(Newline);
        n->children.push_back( expression() );

        while(!match(RightParen)){
            consume(Comma);
            match(Newline);
            n->children.push_back( expression() );
        }
    }

    return n;
}

Parser::Node* Parser::escape(){
    if(match({Binomial, Fraction})) return escapeBinary();
    else if(match(Cases)) return escapeCases();
    else if(match(Matrix)) return escapeMatrix();
    else if(match(Subscript)) return escapeSubscript();
    else if(match(Superscript)) return escapeSuperscript();
    else if(match(Root)) return escapeRoot();
    else fatalError("Invalid escape code");
}

Parser::Node* Parser::escapeBinary(){
    QChar c = source[tokens[token_index-1].start];
    consume(SpecialOpen);
    Node* num = expression();
    consume(SpecialClose);
    consume(SpecialOpen);
    Node* den = expression();
    consume(SpecialClose);

    return createNode(c, num, den);
}

Parser::Node* Parser::escapeCases(){
    Node* n = createNode('C');

    do{
        consume(SpecialOpen);
        n->children.push_back(expression());
        consume(SpecialClose);
        consume(SpecialOpen);
        n->children.push_back(statement());
        consume(SpecialClose);
    } while(peek(SpecialOpen));

    return n;
}

Parser::Node* Parser::escapeMatrix(){
    Node* expr = createNode(QChar(8862));
    consume(SpecialOpen);
    consume(Number);
    bool success;
    QString::size_type start = tokens[token_index-1].start;
    QString::size_type end = tokens[token_index-1].end;
    uint rows = source.mid(start, end-start).toUInt(&success);
    Q_ASSERT(success);
    consume(SpecialClose);

    consume(SpecialOpen);
    consume(Number);
    start = tokens[token_index-1].start;
    end = tokens[token_index-1].end;
    uint cols = source.mid(start, end-start).toUInt(&success);
    Q_ASSERT(success);
    expr->subtext = source.mid(start, end-start);
    consume(SpecialClose);

    for(uint i = 0; i < rows*cols; i++){
        consume(SpecialOpen);
        expr->children.push_back( expression() );
        consume(SpecialClose);
    }

    return expr;
}

Parser::Node* Parser::escapeSubscript(){
    consume(SpecialOpen);
    std::vector<Token>::size_type marker = token_index;
    skipPastSpecialClose();
    consume(SpecialOpen);

    if(match(Multiply)){
        consume(SpecialClose);
        std::vector<Token>::size_type end_marker = token_index;

        token_index = marker;
        consume(Identifier);
        consume(SpecialClose);
        token_index -= 2;
        Node* id = terminal();
        id->subtext += "_*";
        token_index = end_marker;

        return id;
    }else if(match(Comma)){
        //Subscript partial derivative
        consume(Identifier);
        token_index--;
        Node* wrt = terminal();
        consume(SpecialClose);
        std::vector<Token>::size_type end_marker = token_index;

        token_index = marker;
        Node* expr = createNode('d', rightUnary(), wrt);
        consume(SpecialClose);
        token_index = end_marker;

        return expr;
    }else{
        //Subscript access
        token_index = marker;
        Node* lhs = leftUnary();
        consume(SpecialClose);
        consume(SpecialOpen);
        Node* expr = createNode('_', lhs, expression());
        while(match(Comma))
            expr->children.push_back(expression());
        consume(SpecialClose);

        //Note: this could be part of identifier if lhs is id and subscript is single id,
        //      e.g. u = Kₚ * e

        return expr;
    }
}

Parser::Node* Parser::escapeSuperscript(){
    consume(SpecialOpen);
    std::vector<Token>::size_type marker = token_index;
    skipPastSpecialClose();
    consume(SpecialOpen);

    if(match(Multiply)){
        consume(SpecialClose);
        std::vector<Token>::size_type end_marker = token_index;

        token_index = marker;
        consume(Identifier);
        consume(SpecialClose);
        token_index -= 2;
        Node* id = terminal();
        id->subtext += "^*";
        token_index = end_marker;

        return id;
    }else if(match( {Transpose, Dagger} )){
        QChar c = source[tokens[token_index-1].start];
        consume(SpecialClose);
        std::vector<Token>::size_type end_marker = token_index;

        token_index = marker;
        Node* expr = createNode(c, rightUnary());
        consume(SpecialClose);
        token_index = end_marker;

        return expr;
    }else if(peek( {Plus, Minus} )){
        QChar c;
        if(match(Plus)) c = QChar(8314);
        else if(match(Minus)) c = QChar(8315);

        consume(SpecialClose);
        std::vector<Token>::size_type end_marker = token_index;

        token_index = marker;

        Node* expr;
        if(match(Real)){
            expr = createNode(source[tokens[token_index-1].start]);
            expr->subtext = c;
            consume(SpecialClose);
        }else{
            expr = createNode(c, rightUnary());
            consume(SpecialClose);
        }
        token_index = end_marker;

        return expr;
    }else{
        token_index = marker;
        Node* lhs = leftUnary();
        consume(SpecialClose);
        consume(SpecialOpen);
        Node* expr = createNode('^', lhs, expression());
        expr->subtext = "typeset";
        consume(SpecialClose);

        return expr;
    }
}

Parser::Node* Parser::escapeRoot(){
    consume(SpecialOpen);
    Node* expr = createNode(QChar(8730), expression());
    consume(SpecialClose);
    if(match(SpecialOpen)){
        expr->children.push_back(expression());
        consume(SpecialClose);
    }

    return expr;
}

Parser::Node* Parser::terminal(){
    if(match({Integer, Natural, Rational, Real, Quaternion}))
        return createNode(source[tokens[token_index-1].start]);

    Node* n = new Node();
    QString::size_type start = tokens[token_index].start;
    QString::size_type end = tokens[token_index].end;
    n->subtext = source.mid(start, end-start);

    if(match(Number)){
        n->label = 'N';

        if(match(LeftParen)){
            n = createNode('*', n, expression());
            consume(RightParen);
            n->subtext = "No Operator";
        }else if(peek(Identifier)){
            n = createNode('*', n, terminal());
            n->subtext = "No Operator";
        }
    }else if(match(Identifier)){
        n->label = 'I';

        if(match(LeftParen)){
            n->label = 'F';
            n->children.push_back( callArgs() );
        }else if(peek(Identifier)){
            //This implicit multiplication still requires a space unless we limit identifiers
            //to a single character
            n = createNode('*', n, terminal());
            n->subtext = "No Operator";
        }
    }else{
        fatalError("Invalid token");
    }

    return n;
}

void Parser::scan(){
    while(source_index < source.size()){
        switch(source[source_index++].unicode()){
            case ' ':   break;
            case '\t':  break;
            case '@':   emitToken(At); break;
            case '\\':  emitToken(Backslash); break;
            case '|':   emitToken(Bar); break;
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

void Parser::scanEscapeCode(){
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

void Parser::scanConstruct(){
    //DO THIS
}

void Parser::scanNumber(){
    QString::size_type start = source_index - 1;

    if(source_index < source.size())
        while(source[source_index].isDigit() && advanceScanner());
    if(source_index < source.size() && source[source_index] == '.')
        while(advanceScanner() && source[source_index].isDigit());

    emitToken(Number, start);
}

void Parser::scanText(){
    QString::size_type start = source_index - 1;

    if(source_index < source.size())
        while(isIdentifierQChar(source[source_index]) && advanceScanner());
    QString str = source.mid(start, source_index-start);

    auto keyword_lookup = keywords.find(str);
    if(keyword_lookup == keywords.end()) emitToken(Identifier, start);
    else emitToken(keyword_lookup.value(), start);
}

bool Parser::isIdentifierQChar(const QChar& c) const{
    return c.isLetter() || c.isNumber() || c == '_' || (c >= 913 && c <= 937) || (c >= 945 && c <= 969);
}

bool Parser::advanceScanner(){
    return ++source_index < source.size();
}

void Parser::emitToken(const Parser::TokenType& t){
    emitToken(t, source_index-1);
}

void Parser::emitToken(const Parser::TokenType& t, QString::size_type start){
    Token token;
    token.type = t;
    token.start = start;
    token.end = source_index;

    tokens.push_back(token);
}
