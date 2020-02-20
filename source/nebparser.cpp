#include "nebparser.h"

namespace Neb {

Parser::Parser(const QString& source)
    : source(source),
      scanner(source),
      tokens(scanner.tokens) {
    //DO NOTHING
}

std::vector<Node*> Parser::parse(){
    try{
        scanner.scan();
    }catch(int code){
        if(code == 646) err_msg = scanner.err_msg;
        throw code;
    }

    while(match(Newline));
    while(token_index < tokens.size()){
        statements.push_back( statement() );
        while(!dangling_nodes.empty()) dangling_nodes.pop();
        if(token_index < tokens.size()) consume({Newline, Comma});
        while(match(Newline));
    }

    return statements;
}

Token Parser::lastExaminedToken(){
    if(token_index < tokens.size()) return tokens[token_index];
    else return tokens.back();
}

QString Parser::getErrorMessage(){
    Q_ASSERT(!err_msg.isEmpty());
    return err_msg;
}

void Parser::fatalError(const QString& msg){
    err_msg = "Parser Error:\n" + msg;

    //Clean up dynamically allocated memory
    for(Node* n : statements) NodeFunction::deletePostorder(n);
    while(!dangling_nodes.empty()){
        delete dangling_nodes.top();
        dangling_nodes.pop();
    }

    throw 646;
}

Node* Parser::createNode(const NodeType& type){
    Node* n = new Node;
    n->type = type;
    dangling_nodes.push(n);

    return n;
}

Node* Parser::createNode(const NodeType& type, Node* child){
    Node* n = new Node;
    n->type = type;
    n->children.push_back(child);
    dangling_nodes.push(n);

    return n;
}

Node* Parser::createNode(const NodeType& type, Node* lhs, Node* rhs){
    Node* n = new Node;
    n->type = type;
    n->children.push_back(lhs);
    n->children.push_back(rhs);
    dangling_nodes.push(n);

    return n;
}

Node* Parser::createNode(const NodeType& type, std::vector<Node*> children){
    Node* n = new Node;
    n->type = type;
    n->children = children;
    dangling_nodes.push(n);

    return n;
}

void Parser::consume(const TokenType& t){
    if(token_index >= tokens.size()) fatalError("Reached end of token stream while scanning");
    if(t != tokens[token_index++].type){
        token_index--;
        fatalError(QString("Expected token type '") + token_names[t] + "', got type '"
                   + token_names[tokens[token_index].type] + "'");
    }
}

void Parser::consume(const std::vector<TokenType>& types){
    if(token_index >= tokens.size()) fatalError("Reached end of token stream while scanning");
    TokenType t = tokens[token_index++].type;
    for(TokenType type : types) if(type == t) return;
    token_index--;
    fatalError(QString("Invalid token type '") + token_names[t] + "'");
}

bool Parser::match(const TokenType& t){
    if(token_index >= tokens.size()) return false;
    if(t == tokens[token_index].type){
        token_index++;
        return true;
    }else{
        return false;
    }
}

bool Parser::match(const std::vector<TokenType>& types){
    if(token_index >= tokens.size()) return false;
    TokenType t = tokens[token_index].type;
    for(TokenType type : types) if(type == t){
        token_index++;
        return true;
    }

    return false;
}

bool Parser::peek(const TokenType& t) const{
    return token_index < tokens.size() && t == tokens[token_index].type;
}

bool Parser::peek(const std::vector<TokenType>& types) const{
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
            fatalError("Reached end of file scanning for SpecialClose "
                       "(Call from parser.cpp " + QString::number(__LINE__) + ")");
        }
    }
}

Node* Parser::statement(){
    Node* n = expression();

    if(match(Equals)){
        match(Newline);
        n = createNode(EQUAL, n, expression());
        while(match(Equals)){
            match(Newline);
            n->children.push_back(expression());
        }

        return n;
    }else if(match({Less, LessEqual})){
        bool inclusive = tokens[token_index-1].type == LessEqual;
        match(Newline);
        n = createNode(inclusive ? LESS_EQUAL : LESS, n, expression());
        while(match({Less, LessEqual})){
            bool inclusive = tokens[token_index-1].type == LessEqual;
            match(Newline);
            n = createNode(inclusive ? LESS_EQUAL : LESS, n, expression());
        }

        return n;
    }else if(match({Greater, GreaterEqual})){
        bool inclusive = tokens[token_index-1].type == GreaterEqual;
        match(Newline);
        n = createNode(inclusive ? GREATER_EQUAL : GREATER, n, expression());
        while(match({Greater, GreaterEqual})){
            bool inclusive = tokens[token_index-1].type == GreaterEqual;
            match(Newline);
            n = createNode(inclusive ? GREATER_EQUAL : GREATER, n, expression());
        }

        return n;
    }else if(match(In)){
        return createNode(IN, n, expression());
    }else if(match(DefEquals)){
        return createNode(DEFINE_EQUALS, n, expression());
    }else if(match(NotEqual)){
        return createNode(NOT_EQUAL, n, expression());
    }else{
        return createNode(EXPR_STMT, n);
    }
}

Node* Parser::expression(){
    return conjunction();
}

Node* Parser::conjunction(){
    Node* expr = disjunction();
    if(match(Conjunction)){
        match(Newline);
        expr = createNode(LOGICAL_AND, expr, disjunction());
        while(match(Conjunction)){
            match(Newline);
            expr->children.push_back(disjunction());
        }
    }

    return expr;
}

Node* Parser::disjunction(){
    Node* expr = addition();
    if(match(Disjunction)){
        match(Newline);
        expr = createNode(LOGICAL_OR, expr, addition());
        while(match(Disjunction)){
            match(Newline);
            expr->children.push_back(addition());
        }
    }

    return expr;
}

Node* Parser::addition(){ //Left associative (subtraction is anti-commutative)
    Node* expr = multiplication();
    while(match({Plus, Minus, Cap, Cup})){
        TokenType t = tokens[token_index-1].type;
        match(Newline);
        Node* rhs = multiplication();
        switch(t){
            case Plus: expr = createNode(ADDITION, expr, rhs); break;
            case Minus: expr = createNode(SUBTRACTION, expr, rhs); break;
            case Cap: expr = createNode(INTERSECTION, expr, rhs); break;
            case Cup: expr = createNode(UNION, expr, rhs); break;
            default: break;
        }
    }

    return expr;
}

Node* Parser::multiplication(){
    Node* expr = leftUnary();
    while(match({Backslash, Divide, DotProduct, Forwardslash, Multiply, Percent, Times})){
        TokenType t = tokens[token_index-1].type;
        match(Newline);
        Node* rhs = leftUnary();
        switch(t){
            case Backslash: expr = createNode(BACKSLASH, expr, rhs); break;
            case Divide: expr = createNode(DIVIDE, expr, rhs); break;
            case DotProduct: expr = createNode(DOT, expr, rhs); break;
            case Forwardslash: expr = createNode(FORWARDSLASH, expr, rhs); break;
            case Multiply: expr = createNode(MULTIPLICATION, expr, rhs); break;
            case Percent: expr = createNode(MODULUS, expr, rhs); break;
            case Times: expr = createNode(CROSS, expr, rhs); break;
            default: break;
        }
    }

    return expr;
}

Node* Parser::leftUnary(){
    if(match(Minus)){
        return createNode(UNARY_MINUS, rightUnary());
    }else if(match(Not)){
        return createNode(LOGICAL_NOT, rightUnary());
    }else{
        return rightUnary();
    }
}

Node* Parser::rightUnary(){
    Node* expr = exponent();
    while(peek( {Exclam, DoubleExclam, Tick} )){
        if(match(Exclam)){
            if(peek(Exclam)){
                int degree = 1;
                while(match(Exclam)) degree++;
                Node* n = createNode(MULTIFACTORIAL, expr);
                n->subtext = QString::number(degree);

                return n;
            }else{
                return createNode(FACTORIAL, expr);
            }
        }else{
            consume(Tick);
            return createNode(TICK_DERIVATIVE, expr);
        }
    }

    return expr;
}

Node* Parser::exponent(){
    Node* expr = grouping();
    if(match(Caret))
        expr = createNode(POWER, expr, leftUnary());
    return expr;
}

Node* Parser::grouping(){
    if(match(LeftParen)){
        Node* expr = expression();
        if(match(Comma)){
            //Exclusive start of range
            Node* range_end = expression();

            if(match(RightParen)){
                return createNode(RANGE_OPEN_OPEN, expr, range_end);
            }else{
                consume(RightBracket);
                return createNode(RANGE_OPEN_CLOSE, expr, range_end);
            }
        }else{
            consume(RightParen);
            return createNode(PAREN_GROUPING, expr);
        }
    }else if(match(LeftBracket)){
        Node* expr = expression();
        if(match(Comma)){
            //Inclusive start of range
            Node* range_end = expression();

            if(match(RightParen)){
                return createNode(RANGE_CLOSE_OPEN, expr, range_end);
            }else{
                consume(RightBracket);
                return createNode(RANGE_CLOSE_CLOSE, expr, range_end);
            }
        }else{
            consume(RightBracket);
            return createNode(BRACKET_GROUPING, expr);
        }
    }else if(match(LeftBrace)){
        Node* expr = createNode(SET_LITERAL, expression());
        while(match(Comma)){
            match(Newline);
            expr->children.push_back( expression() );
        }
        consume(RightBrace);

        return expr;
    }else if(match(Bar)){
        Node* expr = expression();
        consume(Bar);
        return createNode(ABS, expr);
    }else if(match(DoubleBar)){
        Node* expr = expression();
        consume(DoubleBar);
        return createNode(NORM, expr);
    }else if(match(SpecialEscape)){
        return escape();
    }else{
        return terminal();
    }
}

Node* Parser::callArgs(){
    Node* n = createNode(ARGS);

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

Node* Parser::escape(){
    if(match({Binomial, Fraction})) return escapeBinary();
    else if(match(Cases)) return escapeCases();
    else if(match(Matrix)) return escapeMatrix();
    else if(match(Subscript)) return escapeSubscript();
    else if(match(Superscript)) return escapeSuperscript();
    else if(match(Root)) return escapeRoot();
    else if(match(UnderscriptedWord)) return escapeUnderscriptedWord();
    else fatalError("Invalid escape code (Call from parser.cpp " + QString::number(__LINE__) + ")");
}

Node* Parser::escapeBinary(){
    bool fraction = (tokens[token_index-1].type == Fraction);
    consume(SpecialOpen);
    Node* num = expression();
    consume(SpecialClose);
    consume(SpecialOpen);
    Node* den = expression();
    consume(SpecialClose);

    return createNode(fraction ? TYPED_FRACTION : TYPED_BINOMIAL, num, den);
}

Node* Parser::escapeCases(){
    Node* n = createNode(TYPED_CASES);

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

Node* Parser::escapeMatrix(){
    Node* expr = createNode(TYPED_MATRIX);
    consume(SpecialOpen);
    consume(Number);
    bool success;
    QString::size_type start = tokens[token_index-1].start;
    QString::size_type end = tokens[token_index-1].end;
    uint rows = source.mid(start, end-start).toUInt(&success);
    if(!success) fatalError("Error parsing matrix rows");
    consume(SpecialClose);

    consume(SpecialOpen);
    consume(Number);
    start = tokens[token_index-1].start;
    end = tokens[token_index-1].end;
    uint cols = source.mid(start, end-start).toUInt(&success);
    if(!success) fatalError("Error parsing matrix cols");
    expr->subtext = source.mid(start, end-start);
    consume(SpecialClose);

    for(uint i = 0; i < rows*cols; i++){
        consume(SpecialOpen);
        expr->children.push_back( expression() );
        consume(SpecialClose);
    }

    return expr;
}

Node* Parser::escapeSubscript(){
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
        Node* expr = createNode(SUBSCRIPT_PARTIAL, rightUnary(), wrt);
        consume(SpecialClose);
        token_index = end_marker;

        return expr;
    }else{
        //Subscript access
        token_index = marker;
        Node* lhs = leftUnary();
        consume(SpecialClose);
        consume(SpecialOpen);
        Node* expr = createNode(SUBSCRIPT_ACCESS, lhs, expression());
        while(match(Comma))
            expr->children.push_back(expression());
        consume(SpecialClose);

        //Note: this could be part of identifier if lhs is id and subscript is single id,
        //      e.g. u = Kₚ * e

        return expr;
    }
}

Node* Parser::escapeUnderscriptedWord(){
    consume(SpecialOpen);
    if(match(Lim)){
        Node* n = createNode(LIMIT);
        consume(SpecialClose);
        consume(SpecialOpen);
        n->children.push_back( expression() );
        consume(RightArrow);
        n->children.push_back( expression() );
        consume(SpecialClose);
        n->children.push_back( expression() );

        return n;
    }else{
        Node* n;
        if(match(Min)) n = createNode(MIN);
        else if(match(Max)) n = createNode(MAX);
        else if(match(Infimum)) n = createNode(INFIMUM);
        else{
            consume(Supremum);
            n = createNode(SUPREMUM);
        }

        consume(SpecialClose);
        consume(SpecialOpen);
        n->children.push_back( statement() );
        consume(SpecialClose);
        n->children.push_back( expression() );

        return n;
    }
}

Node* Parser::escapeSuperscript(){
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
        bool transpose = tokens[token_index-1].type == Transpose;
        consume(SpecialClose);
        std::vector<Token>::size_type end_marker = token_index;

        token_index = marker;
        Node* expr = createNode(transpose ? TRANSPOSE : DAGGER, rightUnary());
        consume(SpecialClose);
        token_index = end_marker;

        return expr;
    }else if(match( {Plus, Minus} )){
        bool plus = tokens[token_index-1].type == Plus;

        consume(SpecialClose);
        std::vector<Token>::size_type end_marker = token_index;

        token_index = marker;

        Node* expr;
        if(match(Real)){
            expr = createNode(plus ? POSITIVE_REALS : NEGATIVE_REALS);
            consume(SpecialClose);
        }else{
            expr = createNode(plus ? INCREMENT : DECREMENT, rightUnary());
            consume(SpecialClose);
        }
        token_index = end_marker;

        return expr;
    }else{
        token_index = marker;
        if(match(Complex)){
            consume(SpecialClose);
            consume(SpecialOpen);
            Node* expr = createNode(CONTINUOUS, expression());
            consume(SpecialClose);

            return expr;
        }else{
            Node* lhs = leftUnary();
            consume(SpecialClose);
            consume(SpecialOpen);
            Node* expr = createNode(TYPED_POWER, lhs, expression());
            consume(SpecialClose);

            return expr;
        }
    }
}

Node* Parser::escapeRoot(){
    consume(SpecialOpen);
    Node* child = expression();
    consume(SpecialClose);
    if(match(SpecialOpen)){
        Node* power = expression();
        consume(SpecialClose);

        return createNode(TYPED_ROOT, child, power);
    }else{
        return createNode(TYPED_SQRT, child);
    }
}

Node* Parser::terminal(){
    if(match(Integer)) return createNode(INTEGERS);
    else if(match(Natural)) return createNode(NATURAL_NUMS);
    else if(match(Rational)) return createNode(RATIONAL_NUMS);
    else if(match(Real)) return createNode(REALS);
    else if(match(Quaternion)) return createNode(QUATERNIONS);
    else if(match(Complex)) return createNode(COMPLEX_NUMS);
    else if(match(Infinity)) return createNode(INFTY);
    else if(match(Boolean)) return createNode(BOOLEANS);

    if(token_index >= tokens.size())
        fatalError("Ran out of tokens (Call from parser.cpp " + QString::number(__LINE__) + ")");

    QString::size_type start = tokens[token_index].start;
    QString::size_type end = tokens[token_index].end;
    QString source_text = source.mid(start, end-start);

    if(match(Number)){
        Node* n = createNode(NUMBER);
        n->subtext = source_text;

        if(match(LeftParen)){
            n = createNode(IMPLICIT_MULTIPLY, n, expression());
            consume(RightParen);
        }else if(peek(Identifier)){
            n = createNode(IMPLICIT_MULTIPLY, n, terminal());
        }

        return n;
    }else if(match(Identifier)){
        Node* n = createNode(IDENTIFIER);
        n->subtext = source_text;

        if(match(LeftParen)){
            n->type = CALL;
            n->children.push_back( callArgs() );
        }else if(peek(Identifier)){
            //This implicit multiplication requires a space unless we limit identifiers to 1 char
            n = createNode(IMPLICIT_MULTIPLY, n, terminal());
        }

        return n;
    }else{
        fatalError("Invalid token (Call from parser.cpp " + QString::number(__LINE__) + ")");
    }
}

}
