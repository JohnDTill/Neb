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
        while(match(Equals)) n->children.push_back(expression());

        return n;
    }else if(match(In)){
        QChar c = source[tokens[token_index-1].start];
        return createNode(c, n, expression());
    }

    fatalError("No valid statement found");
}

Parser::Node* Parser::expression(){
    return addition();
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
    while(match({Backslash, Divide, Dot, Forwardslash, Multiply, Percent, Times})){
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
        consume(RightParen);
        return createNode('(', expr);
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

Parser::Node* Parser::escape(){
    if(match(Fraction)){
        consume(SpecialOpen);
        Node* num = expression();
        consume(SpecialClose);
        consume(SpecialOpen);
        Node* den = expression();
        consume(SpecialClose);
        return createNode('f', num, den);
    }else if(match(Superscript)){
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
            token_index = end_marker;

            return expr;
        }else if(peek( {Plus, Minus} )){
            QChar c;
            if(match(Plus)) c = QChar(8314);
            else if(match(Minus)) c = QChar(8315);

            consume(SpecialClose);
            std::vector<Token>::size_type end_marker = token_index;

            token_index = marker;
            Node* expr = createNode(c, rightUnary());
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
    }else{
        fatalError("Invalid escape code");
    }
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
    }else if(match(Identifier)){
        n->label = 'I';
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
            case 8901:  emitToken(Dot); break;
            case 8214:  emitToken(DoubleBar); break;
            case 8252:  emitToken(DoubleExclam); break;
            case 8709:  emitToken(EmptySet); break;
            case '=':   emitToken(Equals); break;
            case '!':   emitToken(Exclam); break;
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
            case '-':   emitToken(Minus); break;
            case '*':   emitToken(Multiply); break;
            case 8469:  emitToken(Natural); break;
            case '\n':  emitToken(Newline); break;
            case 172:   emitToken(Not); break;
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
        case 'b':   emitToken(Binomial); break;
        case 'c':   emitToken(Cases); break;
        case 'f':   emitToken(Fraction); break;
        case 8862:  emitToken(Matrix); break;
        case 8730:  emitToken(Root); break;
        case '_':   emitToken(Subscript); break;
        case '^':   emitToken(Superscript); break;

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
